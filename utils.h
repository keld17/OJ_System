#ifndef UTILS_H
#define UTILS_H

#include <stdio.h>
#include <stdbool.h>

// ���Ͽ��� �� �� �б� (���� �� true, ���� �� false)
bool read_line(FILE* fp, char* buffer, int buffer_size);

// ���Ͽ� �� �� ���� (���� ����, ���� �� true)
bool write_line(FILE* fp, const char* line);

// ���Ͽ��� Ư�� ��(0-based line_number) ���� (���� �� true)
bool delete_line(const char* filename, int line_number);

typedef enum {
    LOGIN_NONE = 0,
    LOGIN_STUDENT = 1,
    LOGIN_ADMIN = 2
} LoginStatus;

typedef enum {
    MENU_EXIT = 0,
    MENU_LOGOUT = 1,
    MENU_USERINFO = 2,
} MenuOption;
#endif
