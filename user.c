#define _CRT_SECURE_NO_WARNINGS
#include "user.h"
#include "utils.h"
#include <stdio.h>
#include <string.h>

// 로그인 구현
int login(UserInfo* user) {
    const char* files[] = {
        "",
        "data/accounts/student.txt",
        "data/accounts/admin.txt"
    };
    char inputID[17], inputPW[17];

    printf("ID를 입력하세요: ");
    if (fgets(inputID, sizeof(inputID), stdin)) {
        inputID[strcspn(inputID, "\n")] = '\0'; // 개행 문자 제거
    }

    printf("비밀번호를 입력하세요: ");
    if (fgets(inputPW, sizeof(inputPW), stdin)) {
        inputPW[strcspn(inputPW, "\n")] = '\0'; // 개행 문자 제거
    }

    for (int i = 1; i < 3; ++i) {
        FILE* fp = fopen(files[i], "r");
        if (fp == NULL) {
            printf("계정 파일을 열 수 없습니다: %s\n", files[i]);
            continue;
        }
        char line[64];
        while (fgets(line, sizeof(line), fp)) {
            // 첫 줄에서 BOM 제거
            if (line[0] == (char)0xEF && line[1] == (char)0xBB && line[2] == (char)0xBF) {
                memmove(line, line + 3, strlen(line) - 2);
            }
            char fileID[17], filePW[17];
            if (sscanf(line, "%16[^/]/%16s", fileID, filePW) == 2) {
                fileID[sizeof(fileID) - 1] = '\0';
                filePW[strcspn(filePW, "\r\n")] = '\0';
                if (strcmp(fileID, inputID) == 0 && strcmp(filePW, inputPW) == 0) {
                    strncpy(user->ID, fileID, sizeof(user->ID));
                    strncpy(user->passWord, filePW, sizeof(user->passWord));
                    fclose(fp);
                    printf("로그인 성공!\n");
                    return i;
                }
            }
            printf("입력ID: '%s', 파일ID: '%s'\n", inputID, fileID);
            printf("입력PW: '%s', 파일PW: '%s'\n", inputPW, filePW);
        }
        fclose(fp);
    }
    printf("로그인 실패: ID 또는 비밀번호가 올바르지 않습니다.\n");
    return 0;
}

// 내 정보 보기 구현
int showUserInfo(UserInfo* user) {
    int input = -1;
    printf("\n===== 내 정보 =====\n");
    printf("ID: %s\n", user->ID);
    printf("비밀번호: %s\n", user->passWord);
    printf("===================\n");
    printf("1. 비밀번호 변경\n");
    printf("2. 계정 삭제\n");
    printf("0. 뒤로가기\n");
    printf("선택: ");
    scanf("%d", &input);
    while (getchar() != '\n'); // 입력 버퍼 비우기
    return input;
}

// 비밀번호 변경 구현
void changePassword(UserInfo* user) {
    char newPW[17], confirmPW[17];
    printf("새 비밀번호를 입력하세요: ");
    if (scanf("%16s", newPW) == 1) {
        newPW[sizeof(newPW) - 1] = '\0'; // Null-terminate to ensure safety
        // 입력 버퍼에 남은 문자가 있으면(16자 이상 입력) 오류 처리
        int ch, tooLong = 0;
        while ((ch = getchar()) != '\n' && ch != EOF) {
            tooLong = 1;
        }
        if (tooLong) {
            printf("오류: 비밀번호는 16자 이하여야 합니다.\n");
            return;
        }
    } else {
        newPW[0] = '\0'; // Ensure null-termination in case of input failure
    }
    printf("새 비밀번호를 한 번 더 입력하세요: ");
    if (scanf("%16s", confirmPW) == 1) {
        confirmPW[sizeof(confirmPW) - 1] = '\0'; // Null-terminate to ensure safety
        // 입력 버퍼에 남은 문자가 있으면(16자 이상 입력) 오류 처리
        int ch, tooLong = 0;
        while ((ch = getchar()) != '\n' && ch != EOF) {
            tooLong = 1;
        }
        if (tooLong) {
            printf("오류: 비밀번호는 16자 이하여야 합니다.\n");
            return;
        }
    } else {
        confirmPW[0] = '\0'; // Ensure null-termination in case of input failure
    }
    if (strcmp(newPW, confirmPW) != 0) {
        printf("비밀번호가 일치하지 않습니다.\n");
        return;
    }

    // 파일 목록
    const char* files[] = {
        "data/accounts/student.txt",
        "data/accounts/admin.txt"
    };

    int found = 0;
    for (int i = 0; i < 2; ++i) {
        FILE* fp = fopen(files[i], "r");
        if (!fp) continue;

        FILE* temp = fopen("data/accounts/temp.txt", "w");
        if (!temp) {
            fclose(fp);
            printf("임시 파일을 열 수 없습니다.\n");
            return;
        }

        char line[64];
        while (fgets(line, sizeof(line), fp)) {
            // 첫 줄에서 BOM 제거
            if (line[0] == (char)0xEF && line[1] == (char)0xBB && line[2] == (char)0xBF) {
                memmove(line, line + 3, strlen(line) - 2);
            }
            char fileID[17], filePW[17];
            if (sscanf(line, "%16[^/]/%16s", fileID, filePW) == 2) {
                filePW[strcspn(filePW, "\r\n")] = 0;
                if (strcmp(fileID, user->ID) == 0) {
                    fprintf(temp, "%s/%s\n", fileID, newPW);
                    found = 1;
                } else {
                    fprintf(temp, "%s/%s\n", fileID, filePW);
                }
            }
        }
        fclose(fp);
        fclose(temp);

        if (found) {
            remove(files[i]);
            rename("data/accounts/temp.txt", files[i]);
            strncpy(user->passWord, newPW, sizeof(user->passWord));
            printf("비밀번호가 성공적으로 변경되었습니다.\n");
            break;
        } else {
            remove("data/accounts/temp.txt");
        }
    }
    if (!found) {
        printf("비밀번호 변경 실패: 계정을 찾을 수 없습니다.\n");
    }
}

// 계정 삭제 구현
void deleteAccount(UserInfo* user) {
    char confirm[4];
    printf("정말로 계정을 삭제하시겠습니까? (yes/no): ");
    scanf("%3s", confirm);
    if (strcmp(confirm, "yes") != 0) {
        printf("계정 삭제가 취소되었습니다.\n");
        while (getchar() != '\n'); // 입력 버퍼 비우기
        return;
    }
    while (getchar() != '\n'); // 입력 버퍼 비우기

    const char* files[] = {
        "data/accounts/student.txt",
        "data/accounts/admin.txt"
    };

    int found = 0;
    for (int i = 0; i < 2; ++i) {
        FILE* fp = fopen(files[i], "r");
        if (!fp) continue;

        FILE* temp = fopen("data/accounts/temp.txt", "w");
        if (!temp) {
            fclose(fp);
            printf("임시 파일을 열 수 없습니다.\n");
            return;
        }

        char line[64];
        while (fgets(line, sizeof(line), fp)) {
            // 첫 줄에서 BOM 제거
            if (line[0] == (char)0xEF && line[1] == (char)0xBB && line[2] == (char)0xBF) {
                memmove(line, line + 3, strlen(line) - 2);
            }
            char fileID[17], filePW[17];
            if (sscanf(line, "%16[^/]/%16s", fileID, filePW) == 2) {
                filePW[strcspn(filePW, "\r\n")] = 0;
                if (strcmp(fileID, user->ID) == 0) {
                    found = 1; // 삭제 대상, 파일에 기록하지 않음
                } else {
                    fprintf(temp, "%s/%s\n", fileID, filePW);
                }
            }
        }
        fclose(fp);
        fclose(temp);

        if (found) {
            remove(files[i]);
            rename("data/accounts/temp.txt", files[i]);
            printf("계정이 성공적으로 삭제되었습니다.\n");
            // user 구조체 초기화
            memset(user->ID, 0, sizeof(user->ID));
            memset(user->passWord, 0, sizeof(user->passWord));
            break;
        } else {
            remove("data/accounts/temp.txt");
        }
    }
    if (!found) {
        printf("계정 삭제 실패: 계정을 찾을 수 없습니다.\n");
    }
}

// 사용자 ID 존재 여부 확인
int findUser(char userID[]) {
    // Example implementation: Check if the userID matches a predefined ID
    const char predefinedID[] = "exampleID";
    if (strcmp(userID, predefinedID) == 0) {
        return 1; // User found
    }
    return 0; // User not found
}