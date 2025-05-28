#ifndef USER_H
#define USER_H

typedef struct {
    char ID[17];
    char passWord[17];
} UserInfo;

int login(UserInfo*);
int showUserInfo(UserInfo*);
void changePassword(UserInfo*);
void deleteAccount(UserInfo*);
int findUser(char[]);

#endif // USER_H