#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "problem.h"
#include "utils.h"

int showProblemInfo(int problemNum) {
    char filePath[128];
    snprintf(filePath, sizeof(filePath), "data/problems/problem%d/problem%d_vector.txt", problemNum, problemNum);

    FILE* fp = fopen(filePath, "r");
    if (!fp) {
        printf("문제 파일을 열 수 없습니다: %s\n", filePath);
        return 0;
    }

    printf("\n===== 문제 %d =====\n", problemNum);
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
    while (getchar() != '\n'); // 입력 버퍼 비우기
    return input;
}

void submit(int problemNum, char ID[]) {
    char c_file_path[256], txt_file_path[256], timestamp[32];
    char file_base[128];

    // 시간 문자열 획득
    if (!get_current_time(timestamp, sizeof(timestamp))) {
        printf("시간 정보를 가져오지 못했습니다.\n");
        return;
    }

    // 파일 이름 구성 (예: user1_20250512.c)
    snprintf(file_base, sizeof(file_base), "%s_%s", ID, timestamp);
    snprintf(c_file_path, sizeof(c_file_path), "submissions/%s.c", file_base);
    snprintf(txt_file_path, sizeof(txt_file_path), "submissions/%s.txt", file_base);

    // 소스 코드 파일 쓰기
    FILE* fp_c = fopen(c_file_path, "w");
    FILE* fp_txt = fopen(txt_file_path, "w");
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

    printf("제출 완료:\n- %s\n- %s\n", c_file_path, txt_file_path);
}

void grade(int problemNum, char ID[]) {
    char timestamp[32];
    char file_base[128], c_file_path[256], result_path[256];
    char result[2048];
    int score = 0;

    // 시간 정보 획득
    if (!get_current_time(timestamp, sizeof(timestamp))) {
        printf("시간 정보를 가져오지 못했습니다.\n");
        return;
    }

    // 파일 경로 설정
    snprintf(file_base, sizeof(file_base), "%s_%s", ID, timestamp);
    snprintf(c_file_path, sizeof(c_file_path), "submissions/%s.c", file_base);
    snprintf(result_path, sizeof(result_path), "submissions/%s_result.txt", file_base);

    // 컴파일 및 실행
    bool success = compile_and_run_c(c_file_path, result, sizeof(result));
    if (!success) {
        printf("컴파일/실행 실패:\n%s\n", result);
    }
    else {
        printf("실행 결과:\n%s\n", result);
    }

    // 예시 채점 기준: 정답이 "42"
    if (strcmp(result, "42\n") == 0 || strcmp(result, "42") == 0)
        score = 100;

    // 결과 저장
    FILE* fp = fopen(result_path, "w");
    if (fp) {
        fprintf(fp, "Score: %d\n", score);
        fprintf(fp, "Output:\n%s\n", result);
        fclose(fp);
    }

    printf("채점 완료: 점수 = %d점\n", score);
}

