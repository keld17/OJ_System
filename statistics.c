#define _CRT_SECURE_NO_WARNINGS
#include "statistics.h"
#include "utils.h"

int showUserStatus(int problemNum, char ID[]) {
    clear_screen();
    char filePath[128];
    snprintf(filePath, sizeof(filePath),
        "data/problems/problem%d/problem%d_log.txt", problemNum, problemNum);
    FILE* fp = fopen(filePath, "r");
    if (!fp) {
        printf("상태 파일을 찾을 수 없습니다: %s\n", filePath);
        return 0;
    }

    printf("\n===== %s님의 문제 %d 상태 =====\n", ID, problemNum);
    printf("%-20s | %-6s | %-20s\n", "아이디", "점수", "제출시간");
    printf("---------------------+--------+----------------------\n");
    char line[256];
    int found = 0;
    int max_score = -1;
    char max_time[64] = {0};
    while (fgets(line, sizeof(line), fp)) {
        char log_id[64], log_time[64];
        int log_score;
        if (sscanf(line, "%63[^/]/%d/%63[^\n]", log_id, &log_score, log_time) == 3) {
            if (strcmp(log_id, ID) == 0) {
                printf("%-20s | %-6d | %-20s\n", log_id, log_score, log_time);
                found = 1;
                if (log_score > max_score || (log_score == max_score && strcmp(log_time, max_time) > 0)) {
                    max_score = log_score;
                    strcpy(max_time, log_time);
                }
            }
        }
    }
    fclose(fp);
    if (!found) {
        printf("제출 기록이 없습니다.\n");
    }
    printf("=============================\n");

    // 최고점수 C코드 파일 출력
    if (found && max_score >= 0) {
        char submissions_dir[256];
        snprintf(submissions_dir, sizeof(submissions_dir),
            "C:/Users/dlekg/source/repos/OJ_System/data/problems/problem%d/submissions", problemNum);
        char c_file_path[512];
        snprintf(c_file_path, sizeof(c_file_path), "%s/%s_%s.c", submissions_dir, ID, max_time);

        FILE* code_fp = fopen(c_file_path, "r");
        printf("\n최고점수(%d) 제출 C코드:\n", max_score, c_file_path);
        if (code_fp) {
            char code_line[256];
            while (fgets(code_line, sizeof(code_line), code_fp)) {
                printf("%s", code_line);
            }
            fclose(code_fp);
        } else {
            printf("C 코드 파일을 찾을 수 없습니다.\n");
        }
        printf("========================================\n");
    }

    // 0 입력 시까지 반복
    int userInput = 1;
    while (userInput) {
        printf("0을 입력하면 돌아갑니다: ");
        int result = scanf("%d", &userInput);
        if (result != 1) {
            // 숫자가 아닌 입력이 들어온 경우
            printf("숫자를 입력하세요.\n");
            while (getchar() != '\n'); // 입력 버퍼 비우기
            userInput = 1; // 루프 유지
            continue;
        }
        if (userInput == 0) { 
            clear_screen();
            while (getchar() != '\n');
            break;
        }
        else {
            printf("press 0 to undo?\n");
        }
    }
    return 1;
}

void showTotalStatus(int problemNum) {
    char stat_path[256];
    snprintf(stat_path, sizeof(stat_path),
        "C:/Users/dlekg/source/repos/OJ_System/data/problems/problem%d/problem%d_statistics.txt",
        problemNum, problemNum);

    FILE* fp = fopen(stat_path, "r");
    if (!fp) {
        printf("통계 파일을 찾을 수 없습니다: %s\n", stat_path);
        return;
    }

    double avg_max = 0.0, avg_submit = 0.0;
    int overall_max = 0;
    if (fscanf(fp, "%lf/%lf/%d", &avg_max, &avg_submit, &overall_max) == 3) {
        printf("\n===== 문제 %d 통계 =====\n", problemNum);
        printf("전체 학생 평균 최고점수 : %.2f\n", avg_max);
        printf("전체 학생 평균 제출횟수 : %.2f\n", avg_submit);
        printf("전체 최고점수           : %d\n", overall_max);
        printf("========================\n");
    } else {
        printf("통계 파일 형식이 올바르지 않습니다.\n");
    }
    fclose(fp);
}

void calculateAverage(int problemNum) {
    char log_path[256], stat_path[256];
    snprintf(log_path, sizeof(log_path),
        "C:/Users/dlekg/source/repos/OJ_System/data/problems/problem%d/problem%d_log.txt",
        problemNum, problemNum);
    snprintf(stat_path, sizeof(stat_path),
        "C:/Users/dlekg/source/repos/OJ_System/data/problems/problem%d/problem%d_statistics.txt",
        problemNum, problemNum);

    // 학생별 최고점수, 제출횟수 집계
    
    UserStat users[256];
    int user_cnt = 0;
    int overall_max = -1;

    FILE* fp = fopen(log_path, "r");
    if (!fp) return;

    char line[256];
    while (fgets(line, sizeof(line), fp)) {
        char log_id[64], log_time[64];
        int log_score;
        if (sscanf(line, "%63[^/]/%d/%63[^\n]", log_id, &log_score, log_time) == 3) {
            // 전체 최고점수 갱신
            if (log_score > overall_max) overall_max = log_score;

            // 학생별 정보 찾기
            int found = 0;
            for (int i = 0; i < user_cnt; ++i) {
                if (strcmp(users[i].id, log_id) == 0) {
                    users[i].submit_count++;
                    if (log_score > users[i].max_score)
                        users[i].max_score = log_score;
                    found = 1;
                    break;
                }
            }
            if (!found && user_cnt < 256) {
                strcpy(users[user_cnt].id, log_id);
                users[user_cnt].max_score = log_score;
                users[user_cnt].submit_count = 1;
                user_cnt++;
            }
        }
    }
    fclose(fp);

    // 평균 계산
    double avg_max = 0, avg_submit = 0;
    for (int i = 0; i < user_cnt; ++i) {
        avg_max += users[i].max_score;
        avg_submit += users[i].submit_count;
    }
    if (user_cnt > 0) {
        avg_max /= user_cnt;
        avg_submit /= user_cnt;
    }

    // 파일에 저장
    FILE* stat_fp = fopen(stat_path, "w");
    if (stat_fp) {
        fprintf(stat_fp, "%.2f/%.2f/%d\n", avg_max, avg_submit, overall_max >= 0 ? overall_max : 0);
        fclose(stat_fp);
    }
}

void append_log(int problemNum, const char* ID, int score, const char* submissionTime) {
    char log_path[256];
    snprintf(log_path, sizeof(log_path),
        "C:/Users/dlekg/source/repos/OJ_System/data/problems/problem%d/problem%d_log.txt",
        problemNum, problemNum);

    FILE* fp = fopen(log_path, "a");
    if (!fp) return;
    fprintf(fp, "%s/%d/%s\n", ID, score, submissionTime);
    fclose(fp);
}