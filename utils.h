#ifndef UTILS_H
#define UTILS_H

#include <stdio.h>
#include <stdbool.h>


typedef struct {
    char id[17];
    int max_score;
    int submit_count;
} UserStat;

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

// 화면을 지우는 함수
void clear_screen();

// 파일에서 한 줄 읽기 (성공 시 true, 실패 시 false)
bool read_line(FILE* fp, char* buffer, int buffer_size);

// 파일에 한 줄 쓰기 (개행 포함, 성공 시 true)            //안쓰임
bool write_line(FILE* fp, const char* line);

// 파일에서 특정 줄(0-based line_number) 삭제 (성공 시 true)    //안쓰임
bool delete_line(const char* filename, int line_number);

// 원하는 경로에 폴더 생성 (성공 시 true)                        //안쓰임
bool create_folder(const char* dir_path, const char* folder_name);

// 원하는 경로에 C 파일 생성 (성공 시 true)
bool create_c_file(const char* dir_path, const char* file_name);

// 원하는 경로에 텍스트 파일 생성 (성공 시 true)
bool create_text_file(const char* dir_path, const char* file_name);

// C 코드 컴파일,실행하여 출력값을 result에 저장 (성공 시 true)        //안쓰임
bool compile_and_run_c(const char* c_file_path, char* result, int result_size);

// 현재 시간을 "YYYYMMDD_HHMMSS" 문자열로 반환 (result에 저장, 성공 시 true)
bool get_current_time(char* result, int result_size);

#endif
