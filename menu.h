#ifndef MENU_H
#define MENU_H

void handleUserInfo(UserInfo* user);
void handleStudentProblem(int problemNum, UserInfo* user);
void handleAdminProblem(int problemNum);
void studentMenu(UserInfo* user);
void adminMenu(UserInfo* user);
int Menu();

#endif