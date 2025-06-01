#define _CRT_SECURE_NO_WARNINGS
#include "menu.h"
#include "utils.h"

int Menu() {
    int input = -1;
    printf("\n========== 메뉴 ==========\n");
    printf("1. 내 정보 보기\n");
    printf("2. 문제 1\n");
    printf("3. 문제 2\n");
    printf("4. 문제 3\n");
    printf("5. 문제 4\n");
    printf("6. 문제 5\n");
    printf("L. 로그아웃\n");
    printf("0. 종료\n");
    printf("=========================\n");
    printf("원하는 메뉴를 선택하세요: ");

    char buf[8];
    scanf("%7s", buf);

    if (strcmp(buf, "1") == 0) {
        return MENU_USERINFO;
    } else if (strcmp(buf, "2") == 0) {
        return 10 + 1;
    } else if (strcmp(buf, "3") == 0) {
        return 10 + 2;
    } else if (strcmp(buf, "4") == 0) {
        return 10 + 3;
    } else if (strcmp(buf, "5") == 0) {
        return 10 + 4;
    } else if (strcmp(buf, "6") == 0) {
        return 10 + 5;
    } else if (strcmp(buf, "L") == 0 || strcmp(buf, "l") == 0) {
        return MENU_LOGOUT;
    } else if (strcmp(buf, "0") == 0) {
        return MENU_EXIT;
    } else {
        printf("잘못된 입력입니다.\n");
        return -1;
    }
}