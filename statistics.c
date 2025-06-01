#define _CRT_SECURE_NO_WARNINGS
#include "statistics.h"
#include "utils.h"

int showUserStatus(int problemNum, char ID[]) {
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
        printf("\n[최고점수(%d) 제출 C코드: %s]\n", max_score, c_file_path);
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
        scanf("%d", &userInput);
        if (userInput == 0) break;
        else printf("press 0 to undo?\n");
    }
    return 1;
}

void showTotalStatus(int problemNum) {
    char dirPath[128];
    snprintf(dirPath, sizeof(dirPath), "data/problems/problem%d/", problemNum);

    // 학생 목록 파일에서 ID를 읽어옴
    FILE* userList = fopen("data/accounts/student.txt", "r");
    if (!userList) {
        printf("학생 목록 파일을 열 수 없습니다.\n");
        return;
    }

    int totalScore = 0, totalSubmit = 0, userCount = 0;
    char line[128];
    printf("\n===== 문제 %d 전체 상태 =====\n", problemNum);
    while (fgets(line, sizeof(line), userList)) {
        char userID[17], dummyPW[17];
        if (sscanf(line, "%16[^/]/%16s", userID, dummyPW) == 2) {
            char statusPath[256];
            snprintf(statusPath, sizeof(statusPath), "%s%s_status.txt", dirPath, userID);
            FILE* status = fopen(statusPath, "r");
            if (status) {
                int score = 0, submit = 0;
                char statusLine[128];
                while (fgets(statusLine, sizeof(statusLine), status)) {
                    // 예시: "점수: 80, 제출: 3"
                    sscanf(statusLine, "점수: %d, 제출: %d", &score, &submit);
                }
                printf("%s - 점수: %d, 제출: %d\n", userID, score, submit);
                totalScore += score;
                totalSubmit += submit;
                userCount++;
                fclose(status);
            }
        }
    }
    fclose(userList);

    if (userCount > 0) {
        printf("전체 평균 점수: %.2f\n", (double)totalScore / userCount);
        printf("유저당 평균 제출수: %.2f\n", (double)totalSubmit / userCount);
    } else {
        printf("제출한 학생이 없습니다.\n");
    }
    printf("===========================\n");
}

void calculateAverage(int problemNum) {
    
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