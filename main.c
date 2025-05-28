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
    int loginStatus = LOGIN_NONE;    //��α���:0,�л��α���:1,�����ڷα���:2
    UserInfo user;

    while (loginStatus == LOGIN_NONE) {
        loginStatus = login(&user);//�α��εǸ� 1�Ǵ� 2 ��ȯ, user���� ������Ʈ
    }


    if (loginStatus == LOGIN_STUDENT) {
        int StudentMenuReturn;
        do {
            StudentMenuReturn = Menu(); //UI �����ֱ�, �Է°� ��ȯ
            if (StudentMenuReturn == MENU_LOGOUT) {   //�α׾ƿ� (L)
                return main();
            }
            else if (StudentMenuReturn == MENU_USERINFO) {  //���������� ()
                int UserInfoInput;
                do {
                    UserInfoInput = showUserInfo(&user);
                    if (UserInfoInput == 1) {   //��й�ȣ ����
                        changePassword(&user);
                    }
                    else if (UserInfoInput == 2) {  //���� ����
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
            else if (StudentMenuReturn > 10 && StudentMenuReturn < 20) {  //���� ��� ����
                int problemNum = StudentMenuReturn - 10;    //���� n���� 10+n�Է�
                int ProblemInfoInput;

                do {
                    ProblemInfoInput = showProblemInfo(problemNum); //�������� ���� // Coding or Status ���� UI ���� �Է¹ޱ�
                    if (ProblemInfoInput == 1) {    //Coding�ϱ�
                        submit(problemNum, user.ID);   //�Է¹ް� ����
                        grade(problemNum, user.ID);    //ä���ϰ� ����
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
                    else if (ProblemInfoInput == 2) { //Status����
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
        } while (StudentMenuReturn != MENU_EXIT);    //0�̸� �ý��� ����
    }



    else if (loginStatus == LOGIN_ADMIN) {
        int adminMenuReturn;
        do {
            adminMenuReturn = Menu(); //UI �����ֱ�, �Է°� ��ȯ
            if (adminMenuReturn == MENU_LOGOUT) {   //�α׾ƿ�
                return main();
            }
            else if (adminMenuReturn == MENU_USERINFO) {  //����������
                int UserInfoInput;
                do {
                    UserInfoInput = showUserInfo(&user);
                    if (UserInfoInput == 1) {   //��й�ȣ ����
                        changePassword(&user);
                    }
                    else if (UserInfoInput == 2) {  //���� ����
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

            else if (adminMenuReturn > 10 && adminMenuReturn < 20) {  //���� ��� ����
                int problemNum = adminMenuReturn - 10;    //���� n���� 10+n�Է�
                calculateAverage();
                showTotalStatus(problemNum);    //��ü �������, ������ ��� �����
                char searchUser[17] = { 0, };
                do {
                    printf("�˻��ϰ��� �ϴ� �̿����� ID�� �Է��ϼ���. ���� �ܰ�� ���ư��÷��� 0�� �Է��ϼ���\n");
                    printf("ID : ");
                    fgets(searchUser, sizeof(searchUser), stdin);
                    searchUser[strcspn(searchUser, "\n")] = 0; // ���๮�� ����
                    if (strcmp(searchUser, "0") == 0) {
                        printf("���� �ܰ�� ���ư��ϴ�.\n");
                        break;
                    }
                    else {
                        if (findUser(searchUser) == 0) {
                            printf("�̿��ڸ� ã�� �� �����ϴ�.\n");
                        }
                        else {
                            showUserStatus(problemNum, searchUser);
                            int userInput = 1;
                            while (userInput) {
                                scanf("%d", &userInput);
                                getchar();  //���������
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
        } while (adminMenuReturn != MENU_EXIT);    //0�̸� �ý��� ����

    }


    else {
        printf("Something went Wrong :(\n");
        return 1;
    }


    return 0;

}

//int login(UserInfo*);                           //����
//int showUserInfo(UserInfo*);                    //����
//int Menu();                                     //����ö
//void changePassword(UserInfo*);                 //����
//void deleteAccount(UserInfo*);                  //����
//int showProblemInfo(int problemNum);            //����ö
//void submit(int problemNum, char ID[]);         //����ȣ
//void grade(int problemNum, char ID[]);          //����ȣ
//int showUserStatus(int problemNum, char ID[]);  //������
//void showTotalStatus(int problemNum);           //���ؼ�
//int findUser(char[]);                           //������
//void calculateAverage();                        //���ؼ�