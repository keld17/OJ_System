#ifndef UTILS_H
#define UTILS_H

#include <stdio.h>
#include <stdbool.h>

// 파일에서 한 줄 읽기 (성공 시 true, 실패 시 false)
bool read_line(FILE* fp, char* buffer, int buffer_size);

// 파일에 한 줄 쓰기 (개행 포함, 성공 시 true)
bool write_line(FILE* fp, const char* line);

// 파일에서 특정 줄(0-based line_number) 삭제 (성공 시 true)
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
