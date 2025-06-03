#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "user.h"
#include "problem.h"
#include "statistics.h"
#include "menu.h"
#include "utils.h"

int Menu() {
    int input = -1;
    printf("\n========== 메뉴 ==========\n");
    printf("1. 문제 1\n");
    printf("2. 문제 2\n");
    printf("3. 문제 3\n");
    printf("4. 문제 4\n");
    printf("5. 문제 5\n");
    printf("6. 문제 6\n");
    printf("7. 문제 7\n");
    printf("8. 문제 8\n");
    printf("9. 문제 9\n");
    printf("M. 내 정보 보기\n");
    printf("L. 로그아웃\n");
    printf("0. 종료\n");
    printf("=========================\n");
    printf("원하는 메뉴를 선택하세요: ");

    char buf[8];
    scanf("%7s", buf);
    while (getchar() != '\n'); // 입력 버퍼 비우기

    if (strcmp(buf, "1") == 0) {
        clear_screen();
        return 10 + 1;
    } else if (strcmp(buf, "2") == 0) {
        clear_screen();
        return 10 + 2;
    } else if (strcmp(buf, "3") == 0) {
        clear_screen();
        return 10 + 3;
    } else if (strcmp(buf, "4") == 0) {
        clear_screen();
        return 10 + 4;
    } else if (strcmp(buf, "5") == 0) {
        clear_screen();
        return 10 + 5;
    } else if (strcmp(buf, "6") == 0) {
        clear_screen();
        return 10 + 6;
    } else if (strcmp(buf, "7") == 0) {
        clear_screen();
        return 10 + 7;
    } else if (strcmp(buf, "8") == 0) {
        clear_screen();
        return 10 + 8;
    } else if (strcmp(buf, "9") == 0) {
        clear_screen();
        return 10 + 9;
    } else if (strcmp(buf, "M") == 0 || strcmp(buf, "m") == 0) {
        clear_screen();
        return MENU_USERINFO;
    } else if (strcmp(buf, "L") == 0 || strcmp(buf, "l") == 0) {
        clear_screen();
        return MENU_LOGOUT;
    } else if (strcmp(buf, "0") == 0) {
        clear_screen();
        return MENU_EXIT;
    } else {
        printf("잘못된 입력입니다.\n");
        return -1;
    }
}

// 사용자 정보 처리 함수
void handleUserInfo(UserInfo* user) {
    int UserInfoInput;
    do {
        UserInfoInput = showUserInfo(user);
        if (UserInfoInput == 1) {   // 비밀번호 변경
            clear_screen();
            changePassword(user);
        }
        else if (UserInfoInput == 2) {  // 계정 삭제
            deleteAccount(user);
            main(); // 계정 삭제 후 재시작
            exit(0);
        }
        else if (UserInfoInput == 0) {
            clear_screen();
            printf("Going back...\n");
        }
        else {
            clear_screen();
            printf("Wrong Input!\n");
        }
    } while (UserInfoInput != 0);
}

// 학생 문제 처리 함수
void handleStudentProblem(int problemNum, UserInfo* user) {
    char problemPath[128];
    snprintf(problemPath, sizeof(problemPath), "data/problems/problem%d/problem%d_vector.txt", problemNum, problemNum);
    FILE* pf = fopen(problemPath, "r");
    if (!pf) {
        printf("문제 %d에 해당하는 파일이 존재하지 않습니다.\n", problemNum);
        return;
    }
    fclose(pf);

    int ProblemInfoInput;
    while(1) {
        ProblemInfoInput = showProblemInfo(problemNum);
        if (ProblemInfoInput == 0) {
            clear_screen();
            break;
        }
        if (ProblemInfoInput == 1) {    // Coding하기
            char submissionTime[16];
            submit(problemNum, user->ID, submissionTime);
            grade(problemNum, user->ID, submissionTime);
            showUserStatus(problemNum, user->ID);
        }
        else if (ProblemInfoInput == 2) { // Status보기
            clear_screen();
            showUserStatus(problemNum, user->ID);
        }
    }
}

// 관리자 문제 처리 함수
void handleAdminProblem(int problemNum) {
    char problemPath[128];
    snprintf(problemPath, sizeof(problemPath), "data/problems/problem%d/problem%d_vector.txt", problemNum, problemNum);
    FILE* pf = fopen(problemPath, "r");
    if (!pf) {
        printf("문제 %d에 해당하는 파일이 존재하지 않습니다.\n", problemNum);
        return;
    }
    fclose(pf);

    calculateAverage(problemNum);
    do {
        showTotalStatus(problemNum);
        char searchUser[17] = { 0, };
        printf("검색하고자 하는 이용자의 ID를 입력하세요. 이전 단계로 돌아가시려면 0을 입력하세요\n");
        printf("ID : ");
        fgets(searchUser, sizeof(searchUser), stdin);
        searchUser[strcspn(searchUser, "\n")] = 0;
        if (strcmp(searchUser, "0") == 0) {
            clear_screen();
            printf("이전 단계로 돌아갑니다.\n");
            break;
        }
        else {
            if (hasSubmissionRecord(problemNum, searchUser) == 0) {
                clear_screen();
                printf("이용자를 찾을 수 없습니다.\n");
            }
            else {
                showUserStatus(problemNum, searchUser);
            }
        }
    } while (1);
}

// 학생 메뉴 처리 함수
void studentMenu(UserInfo* user) {
    int StudentMenuReturn;
    do {
        StudentMenuReturn = Menu();
        if (StudentMenuReturn == MENU_LOGOUT) {
            main();
            exit(0);
        }
        else if (StudentMenuReturn == MENU_USERINFO) {
            handleUserInfo(user);
        }
        else if (StudentMenuReturn > 10 && StudentMenuReturn < 20) {
            int problemNum = StudentMenuReturn - 10;
            handleStudentProblem(problemNum, user);
        }
        else if (StudentMenuReturn == MENU_EXIT) {
            printf("Turning Off...\n");
        }
        else {
            printf("Wrong Input!\n");
        }
    } while (StudentMenuReturn != MENU_EXIT);
}

// 관리자 메뉴 처리 함수
void adminMenu(UserInfo* user) {
    int adminMenuReturn;
    do {
        adminMenuReturn = Menu();
        if (adminMenuReturn == MENU_LOGOUT) {
            main();
            exit(0);
        }
        else if (adminMenuReturn == MENU_USERINFO) {
            handleUserInfo(user);
        }
        else if (adminMenuReturn > 10 && adminMenuReturn < 20) {
            int problemNum = adminMenuReturn - 10;
            handleAdminProblem(problemNum);
        }
        else if (adminMenuReturn == MENU_EXIT) {
            printf("Turning Off...\n");
        }
        else {
            printf("Wrong Input!\n");
        }
    } while (adminMenuReturn != MENU_EXIT);
}
