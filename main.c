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
    int loginStatus = 0;    //��α���:0,�л��α���:1,�����ڷα���:2
    UserInfo user;

    while (!loginStatus) {
        loginStatus = login(&user);//�α��εǸ� 1�Ǵ� 2 ��ȯ, user���� ������Ʈ
    }


    if (loginStatus == 1) {
        int StudentMenuReturn;
        do {
            StudentMenuReturn = StudentMenu(); //UI �����ֱ�, �Է°� ��ȯ
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
                    else {
                        printf("Wrong Input!\n");
                    }
                } while (UserInfoInput != 0);

            }
            else if (StudentMenuReturn > 10) {  //���� ��� ����
                int problemNum = StudentMenuReturn - 10;    //���� n���� 10+n�Է�
                int ProblemInfoInput;

                do {
                    ProblemInfoInput = showProblemInfo(problemNum); //�������� ���� // Coding or Status ���� UI ���� d�Է¹ޱ�
                    if (ProblemInfoInput == 1) {    //Coding�ϱ�
                        /*�ڵ� Ǯ��*/
                    }
                    else if (ProblemInfoInput == 2) { //Status����
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
        } while (StudentMenuReturn != 0);    //0�̸� �ý��� ����
    }



    else if (loginStatus == 2) {
        int adminMenuReturn;
        do {
            adminMenuReturn = adminMenu(); //UI �����ֱ�, �Է°� ��ȯ
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
                    else {
                        printf("Wrong Input!\n");
                    }
                } while (UserInfoInput != 0);

            }

            else if (adminMenuReturn > 10) {  //���� ��� ����
                int problemNum = adminMenuReturn - 10;    //���� n���� 10+n�Է�
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
                        showUserStatus(&user, problemNum);
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
                } while (1);

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