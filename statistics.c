#define _CRT_SECURE_NO_WARNINGS
#include "statistics.h"
#include "utils.h"

int showUserStatus(int problemNum, char ID[]) {
    char filePath[128];
    snprintf(filePath, sizeof(filePath), "data/problems/problem%d/%s_status.txt", problemNum, ID);

    FILE* fp = fopen(filePath, "r");
    if (!fp) {
        printf("상태 파일을 찾을 수 없습니다: %s\n", filePath);
        return 0;
    }

    printf("\n===== %s님의 문제 %d 상태 =====\n", ID, problemNum);
    char line[256];
    while (fgets(line, sizeof(line), fp)) {
        printf("%s", line);
    }
    fclose(fp);
    printf("=============================\n");
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

void calculateAverage() {
    // 모든 문제에 대해 전체 평균 점수와 제출수 계산
    for (int problemNum = 1; ; ++problemNum) {
        char dirPath[128];
        snprintf(dirPath, sizeof(dirPath), "data/problems/problem%d/", problemNum);
        // 디렉토리 존재 여부 확인
        FILE* test = fopen(dirPath, "r");
        if (!test) break;
        fclose(test);
        showTotalStatus(problemNum);
    }
}