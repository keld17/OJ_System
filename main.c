#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

typedef struct {
    char ID[17];
    char passWord[17];
}UserInfo;


int login(UserInfo*);                           //����
int showUserInfo(UserInfo*);                    //����
int Menu();                                     //����ö
void changePassword(UserInfo*);                 //����
void deleteAccount(UserInfo*);                  //����
int showProblemInfo(int problemNum);            //����ö
void submit(int problemNum, char ID[]);         //����ȣ
void grade(int problemNum, char ID[]);          //����ȣ
int showUserStatus(int problemNum, char ID[]);  //������
void showTotalStatus(int problemNum);           //���ؼ�
int findUser(char[]);                           //������
void calculateAverage();                        //���ؼ�

int main() {
    int loginStatus = 0;    //��α���:0,�л��α���:1,�����ڷα���:2
    UserInfo user;

    while (!loginStatus) {
        loginStatus = login(&user);//�α��εǸ� 1�Ǵ� 2 ��ȯ, user���� ������Ʈ
    }


    if (loginStatus == 1) {
        int StudentMenuReturn;
        do {
            StudentMenuReturn = Menu(); //UI �����ֱ�, �Է°� ��ȯ
            if (StudentMenuReturn == 1) {   //�α׾ƿ� (L)
                return main();
            }
            else if (StudentMenuReturn == 2) {  //���������� ()
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
            else if (StudentMenuReturn == 0) {
                printf("Turning Off...\n");
            }
            else {
                printf("Wrong Input!\n");
            }
        } while (StudentMenuReturn != 0);    //0�̸� �ý��� ����
    }



    else if (loginStatus == 2) {
        int adminMenuReturn;
        do {
            adminMenuReturn = Menu(); //UI �����ֱ�, �Է°� ��ȯ
            if (adminMenuReturn == 1) {   //�α׾ƿ�
                return main();
            }
            else if (adminMenuReturn == 2) {  //����������
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
                    gets(searchUser);
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
            else if (adminMenuReturn == 0) {
                printf("Turning Off...\n");
            }
            else {
                printf("Wrong Input!\n");
            }
        } while (adminMenuReturn != 0);    //0�̸� �ý��� ����

    }


    else {
        printf("Something went Wrong :(\n");
        return 1;
    }


    return 0;

}