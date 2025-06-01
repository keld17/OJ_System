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
    int loginStatus = LOGIN_NONE;
    UserInfo user;

    while (loginStatus == LOGIN_NONE) {
        loginStatus = login(&user);
    }

    if (loginStatus == LOGIN_STUDENT) {
        studentMenu(&user);
    }
    else if (loginStatus == LOGIN_ADMIN) {
        adminMenu(&user);
    }
    else {
        printf("Something went Wrong :(\n");
        return 1;
    }

    return 0;
}