#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "problem.h"
#include "utils.h"
#include "statistics.h"

int showProblemInfo(int problemNum) {
    char filePath[128];
    snprintf(filePath, sizeof(filePath), "data/problems/problem%d/problem%d.txt", problemNum, problemNum);

    FILE* fp = fopen(filePath, "r");
    if (!fp) {
        printf("문제 파일을 열 수 없습니다: %s\n", filePath);
        return 0;
    }

    printf("\n===== 문제 %d =====\n", problemNum);

    // 문제 파일 내용 출력
    char line[256];
    while (fgets(line, sizeof(line), fp)) {
        printf("%s", line);
    }
    fclose(fp);

    printf("====================\n");
    printf("1. Coding하기\n");
    printf("2. Status보기\n");
    printf("0. 뒤로가기\n");
    printf("선택: ");

    int input = -1;
    scanf("%d", &input);
    while (getchar() != '\n');
    return input;
}

void submit(int problemNum, char ID[], char submissionTime[]) {
    char old_c_file_path[512], old_txt_file_path[512], timestamp[32];
    char file_base[128];

    // 시간 문자열 획득
    if (!get_current_time(timestamp, sizeof(timestamp))) {
        printf("시간 정보를 가져오지 못했습니다.\n");
        return;
    }

    // 경로 지정
    char submissions_dir[256];
    snprintf(submissions_dir, sizeof(submissions_dir),
        "C:/Users/dlekg/source/repos/OJ_System/data/problems/problem%d/submissions", problemNum);

    // 파일 이름 구성 (경로 포함)
    snprintf(file_base, sizeof(file_base), "%s_%s", ID, timestamp);
    snprintf(old_c_file_path, sizeof(old_c_file_path), "%s/%s.c", submissions_dir, file_base);
    snprintf(old_txt_file_path, sizeof(old_txt_file_path), "%s/%s.txt", submissions_dir, file_base);

    // 파일 생성
    if (!create_c_file(submissions_dir, file_base)) {
        printf("C 파일 생성에 실패했습니다.\n");
        return;
    }
    if (!create_text_file(submissions_dir, file_base)) {
        printf("텍스트 파일 생성에 실패했습니다.\n");
        return;
    }

    // 소스 코드 파일 쓰기
    FILE* fp_c = fopen(old_c_file_path, "w");
    FILE* fp_txt = fopen(old_txt_file_path, "w");
    if (!fp_c || !fp_txt) {
        printf("파일 열기에 실패했습니다.\n");
        if (fp_c) fclose(fp_c);
        if (fp_txt) fclose(fp_txt);
        return;
    }

    printf("소스코드를 입력하세요. (종료: Ctrl+D 또는 Ctrl+Z)\n");

    char buffer[1024];
    while (fgets(buffer, sizeof(buffer), stdin)) {
        fputs(buffer, fp_c); // .c파일에 저장
        fputs(buffer, fp_txt); // .txt파일에 저장
    }

    fclose(fp_c);
    fclose(fp_txt);

    printf("제출 완료:\n- %s\n- %s\n", old_c_file_path, old_txt_file_path);

    // 93~95행: 새로운 파일명 선언
    char c_file_path[512], txt_file_path[512];

    if (!get_current_time(submissionTime, 32)) {
        printf("제출 시간 정보를 다시 가져오지 못했습니다.\n");
        return;
    }
    snprintf(file_base, sizeof(file_base), "%s_%s", ID, submissionTime);
    snprintf(c_file_path, sizeof(c_file_path), "%s/%s.c", submissions_dir, file_base);
    snprintf(txt_file_path, sizeof(txt_file_path), "%s/%s.txt", submissions_dir, file_base);

    if (rename(old_c_file_path, c_file_path) != 0) {
        printf("C 파일 이름 변경에 실패했습니다.\n");
    }
    if (rename(old_txt_file_path, txt_file_path) != 0) {
        printf("텍스트 파일 이름 변경에 실패했습니다.\n");
    }
}

void grade(int problemNum, char ID[], char submissionTime[]) {
    char file_base[128], c_file_path[512], result_path[512];
    char submissions_dir[256];
    snprintf(submissions_dir, sizeof(submissions_dir),
        "C:/Users/dlekg/source/repos/OJ_System/data/problems/problem%d/submissions", problemNum);
    snprintf(file_base, sizeof(file_base), "%s_%s", ID, submissionTime);
    snprintf(c_file_path, sizeof(c_file_path), "%s/%s.c", submissions_dir, file_base);

    // 입력 벡터 파일 경로
    char vector_path[256];
    snprintf(vector_path, sizeof(vector_path), "data/problems/problem%d/problem%d_vector.txt", problemNum, problemNum);

    // 정답 코드 경로
    char solution_c_path[256];
    snprintf(solution_c_path, sizeof(solution_c_path), "data/problems/problem%d/problem%d_solution.c", problemNum, problemNum);

    FILE* vec_fp = fopen(vector_path, "r");
    if (!vec_fp) {
        printf("입력 벡터 파일을 열 수 없습니다: %s\n", vector_path);
        return;
    }

    int correct = 0;
    char input[256];
    for (int i = 0; i < 10 && fgets(input, sizeof(input), vec_fp); i++) {
        // 입력을 임시 파일에 저장
        FILE* temp_fp = fopen("temp_input.txt", "w");
        if (!temp_fp) continue;
        fputs(input, temp_fp);
        fclose(temp_fp);

        // 정답 코드 실행
        char sol_cmd[512];
        snprintf(sol_cmd, sizeof(sol_cmd), "gcc \"%s\" -o sol_temp.exe 2> nul && sol_temp.exe < temp_input.txt > sol_out.txt", solution_c_path);
        system(sol_cmd);

        // 제출 코드 실행
        char user_cmd[512];
        snprintf(user_cmd, sizeof(user_cmd), "gcc \"%s\" -o user_temp.exe 2> nul && user_temp.exe < temp_input.txt > user_out.txt", c_file_path);
        system(user_cmd);

        // 출력 비교
        FILE* sol_fp = fopen("sol_out.txt", "r");
        FILE* user_fp = fopen("user_out.txt", "r");
        if (sol_fp && user_fp) {
            char sol_buf[256] = {0}, user_buf[256] = {0};
            read_line(sol_fp, sol_buf, sizeof(sol_buf));
            read_line(user_fp, user_buf, sizeof(user_buf));
            // 개행, 공백 무시 비교
            char* s = sol_buf; while (*s == '\n' || *s == '\r' || *s == ' ') s++;
            char* u = user_buf; while (*u == '\n' || *u == '\r' || *u == ' ') u++;
            if (strcmp(s, u) == 0) correct++;
            fclose(sol_fp);
            fclose(user_fp);
        }
        remove("temp_input.txt");
        remove("sol_out.txt");
        remove("user_out.txt");
        remove("sol_temp.exe");
        remove("user_temp.exe");
    }
    fclose(vec_fp);

    int score = correct * 10;
    printf("채점 완료: 점수 = %d점\n", score);

    append_log(problemNum, ID, score, submissionTime);
}
