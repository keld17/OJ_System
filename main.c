#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "utils.h"
#include "user.h"
#include "problem.h"
#include "statistics.h"
#include "menu.h"

int main() {
    int loginStatus = LOGIN_NONE;    //비로그인:0,학생로그인:1,관리자로그인:2
    UserInfo user;

    while (loginStatus == LOGIN_NONE) {
        loginStatus = login(&user);//로그인되면 1또는 2 반환, user정보 업데이트
    }


    if (loginStatus == LOGIN_STUDENT) {
        int StudentMenuReturn;
        do {
            StudentMenuReturn = Menu(); //UI 보여주기, 입력값 반환
            if (StudentMenuReturn == MENU_LOGOUT) {   //로그아웃 (L)
                return main();
            }
            else if (StudentMenuReturn == MENU_USERINFO) {  //내정보보기 ()
                int UserInfoInput;
                do {
                    UserInfoInput = showUserInfo(&user);
                    if (UserInfoInput == 1) {   //비밀번호 변경
                        changePassword(&user);
                    }
                    else if (UserInfoInput == 2) {  //계정 삭제
                        deleteAccount(&user);
                        return main();
                    }
                    else if (UserInfoInput == 0) {
                        printf("Going back...\n");
                    }
                    else {
                        printf("Wrong Input!\n");
                    }
                } while (UserInfoInput != 0);

            }
            else if (StudentMenuReturn > 10 && StudentMenuReturn < 20) {  //문제 목록 보기
                int problemNum = StudentMenuReturn - 10;    //문제 n번은 10+n입력

                // 문제 파일 존재 여부 확인 로직 추가
                char problemPath[128];
                snprintf(problemPath, sizeof(problemPath), "data/problems/problem%d/problem%d_vector.txt", problemNum, problemNum);
                FILE* pf = fopen(problemPath, "r");
                if (!pf) {
                    printf("문제 %d에 해당하는 파일이 존재하지 않습니다.\n", problemNum);
                    continue; // 다음 메뉴로 넘어감
                }
                fclose(pf);

                int ProblemInfoInput;
                do {
                    ProblemInfoInput = showProblemInfo(problemNum); //문제설명 띄우기 // Coding or Status 선택 UI 띄우고 입력받기
                    if (ProblemInfoInput == 1) {    //Coding하기
                        submit(problemNum, user.ID);   //입력받고 저장
                        grade(problemNum, user.ID);    //채점하고 저장
                        showUserStatus(problemNum, user.ID);
                        int userInput = 1;
                        while (userInput) {
                            scanf("%d", &userInput);
                            if (userInput == 0) {
                                break;
                            }
                            else {
                                printf("press 0 to undo?");
                            }
                        }

                    }
                    else if (ProblemInfoInput == 2) { //Status보기
                        showUserStatus(problemNum, user.ID);
                        int userInput = 1;
                        while (userInput) {
                            scanf("%d", &userInput);
                            if (userInput == 0) {
                                break;
                            }
                            else {
                                printf("press 0 to undo?");
                            }
                        }
                    }
                } while (ProblemInfoInput != 0);
            }
            else if (StudentMenuReturn == MENU_EXIT) {
                printf("Turning Off...\n");
            }
            else {
                printf("Wrong Input!\n");
            }
        } while (StudentMenuReturn != MENU_EXIT);    //0이면 시스템 종료
    }



    else if (loginStatus == LOGIN_ADMIN) {
        int adminMenuReturn;
        do {
            adminMenuReturn = Menu(); //UI 보여주기, 입력값 반환
            if (adminMenuReturn == MENU_LOGOUT) {   //로그아웃
                return main();
            }
            else if (adminMenuReturn == MENU_USERINFO) {  //내정보보기
                int UserInfoInput;
                do {
                    UserInfoInput = showUserInfo(&user);
                    if (UserInfoInput == 1) {   //비밀번호 변경
                        changePassword(&user);
                    }
                    else if (UserInfoInput == 2) {  //계정 삭제
                        deleteAccount(&user);
                        return main();
                    }
                    else if (UserInfoInput == 0) {
                        printf("Going back...\n");
                    }
                    else {
                        printf("Wrong Input!\n");
                    }
                } while (UserInfoInput != 0);

            }

            else if (adminMenuReturn > 10 && adminMenuReturn < 20) {  //문제 목록 보기
                int problemNum = adminMenuReturn - 10;    //문제 n번은 10+n입력
                // 문제 파일 존재 여부 확인 로직 추가
                char problemPath[128];
                snprintf(problemPath, sizeof(problemPath), "data/problems/problem%d/problem%d_vector.txt", problemNum, problemNum);
                FILE* pf = fopen(problemPath, "r");
                if (!pf) {
                    printf("문제 %d에 해당하는 파일이 존재하지 않습니다.\n", problemNum);
                    continue; // 다음 메뉴로 넘어감
                }
                fclose(pf);
                calculateAverage();
                showTotalStatus(problemNum);    //전체 평균점수, 유저당 평균 제출수
                char searchUser[17] = { 0, };
                do {
                    printf("검색하고자 하는 이용자의 ID를 입력하세요. 이전 단계로 돌아가시려면 0을 입력하세요\n");
                    printf("ID : ");
                    fgets(searchUser, sizeof(searchUser), stdin);
                    searchUser[strcspn(searchUser, "\n")] = 0; // 개행문자 제거
                    if (strcmp(searchUser, "0") == 0) {
                        printf("이전 단계로 돌아갑니다.\n");
                        break;
                    }
                    else {
                        if (findUser(searchUser) == 0) {
                            printf("이용자를 찾을 수 없습니다.\n");
                        }
                        else {
                            showUserStatus(problemNum, searchUser);
                            int userInput = 1;
                            while (userInput) {
                                scanf("%d", &userInput);
                                getchar();  //버퍼지우기
                                if (userInput == 0) {
                                    break;
                                }
                                else {
                                    printf("press 0 to undo");
                                }
                            }
                        }
                    }
                } while (1);

            }
            else if (adminMenuReturn == MENU_EXIT) {
                printf("Turning Off...\n");
            }
            else {
                printf("Wrong Input!\n");
            }
        } while (adminMenuReturn != MENU_EXIT);    //0이면 시스템 종료

    }


    else {
        printf("Something went Wrong :(\n");
        return 1;
    }


    return 0;

}