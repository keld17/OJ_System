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

// ���ϴ� ��ο� ���� ���� (���� �� true)
bool create_folder(const char* dir_path, const char* folder_name);

// ���ϴ� ��ο� C ���� ���� (���� �� true)
bool create_c_file(const char* dir_path, const char* file_name);

// ���ϴ� ��ο� �ؽ�Ʈ ���� ���� (���� �� true)
bool create_text_file(const char* dir_path, const char* file_name);

// C �ڵ� ������,�����Ͽ� ��°��� result�� ���� (���� �� true)
bool compile_and_run_c(const char* c_file_path, char* result, int result_size);

// ���� �ð��� "YYYYMMDD_HHMMSS" ���ڿ��� ��ȯ (result�� ����, ���� �� true)
bool get_current_time(char* result, int result_size);

#endif
