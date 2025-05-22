#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    char ID[17];
    char passWord[17];
}UserInfo;


int login(UserInfo*);
int StudentMenu();


int main() {
    int loginStatus = 0;    //비로그인:0,학생로그인:1,관리자로그인:2
    UserInfo user;

    while (!loginStatus) {
        loginStatus = login(&user);//로그인되면 1또는 2 반환, user정보 업데이트
    }


    if (loginStatus == 1) {
        int StudentMenuReturn;
        do {
            StudentMenuReturn = StudentMenu(); //UI 보여주기, 입력값 반환
            if (StudentMenuReturn == 1) {   //로그아웃 (L)
                return main();
            }
            else if (StudentMenuReturn == 2) {  //내정보보기 ()
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
                    else {
                        printf("Wrong Input!\n");
                    }
                } while (UserInfoInput != 0);

            }
            else if (StudentMenuReturn > 10) {  //문제 목록 보기
                int problemNum = StudentMenuReturn - 10;    //문제 n번은 10+n입력
                int ProblemInfoInput;

                do {
                    ProblemInfoInput = showProblemInfo(problemNum); //문제설명 띄우기 // Coding or Status 선택 UI 띄우고 d입력받기
                    if (ProblemInfoInput == 1) {    //Coding하기
                        /*코드 풀이*/
                    }
                    else if (ProblemInfoInput == 2) { //Status보기
                        showUserStatus(&user, problemNum);
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
            else {
                printf("Wrong Input!\n");
            }
        } while (StudentMenuReturn != 0);    //0이면 시스템 종료
    }



    else if (loginStatus == 2) {
        int adminMenuReturn;
        do {
            adminMenuReturn = adminMenu(); //UI 보여주기, 입력값 반환
            if (adminMenuReturn == 1) {   //로그아웃
                return main();
            }
            else if (adminMenuReturn == 2) {  //내정보보기
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
                    else {
                        printf("Wrong Input!\n");
                    }
                } while (UserInfoInput != 0);

            }

            else if (adminMenuReturn > 10) {  //문제 목록 보기
                int problemNum = adminMenuReturn - 10;    //문제 n번은 10+n입력
                showTotalStatus(problemNum);    //전체 평균점수, 유저당 평균 제출수
                char searchUser[17] = { 0, };
                do {
                    printf("검색하고자 하는 이용자의 ID를 입력하세요. 이전 단계로 돌아가시려면 0을 입력하세요\n");
                    printf("ID : ");
                    gets(searchUser);
                    if (strcmp(searchUser, "0") == 0) {
                        printf("이전 단계로 돌아갑니다.\n");
                        break;
                    }
                    else {
                        showUserStatus(&user, problemNum);
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
                } while (1);

            }
            else {
                printf("Wrong Input!\n");
            }
        } while (adminMenuReturn != 0);    //0이면 시스템 종료

    }


    else {
        printf("Something went Wrong :(\n");
        return 1;
    }


    return 0;

}