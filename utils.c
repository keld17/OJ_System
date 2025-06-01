
#define _CRT_SECURE_NO_WARNINGS
#include "utils.h"
#include <string.h>
#include <stdio.h>
#include <stdbool.h>
#include <sys/stat.h>
#include <time.h>
#include <stdlib.h>

#ifdef _WIN32
#include <direct.h>
#define PATH_SEPARATOR '\\'
#else
#include <unistd.h>
#define PATH_SEPARATOR '/'
#endif

// 파일에서 한 줄 읽기
bool read_line(FILE* fp, char* buffer, int buffer_size) {
    if (fgets(buffer, buffer_size, fp) == NULL)
        return false;
    // 개행 문자 제거
    buffer[strcspn(buffer, "\r\n")] = '\0';
    return true;
}

// 파일에 한 줄 쓰기
bool write_line(FILE* fp, const char* line) {
    if (fprintf(fp, "%s\n", line) < 0)
        return false;
    return true;
}

// 파일에서 특정 줄 삭제
bool delete_line(const char* filename, int line_number) {
    FILE* fp = fopen(filename, "r");
    if (!fp) return false;

    FILE* temp = fopen("temp.txt", "w");
    if (!temp) {
        fclose(fp);
        return false;
    }

    char buffer[1024];
    int current_line = 0;
    bool deleted = false;

    while (fgets(buffer, sizeof(buffer), fp)) {
        if (current_line != line_number) {
            fputs(buffer, temp);
        } else {
            deleted = true;
        }
        current_line++;
    }

    fclose(fp);
    fclose(temp);

    if (deleted) {
        remove(filename);
        rename("temp.txt", filename);
    } else {
        remove("temp.txt");
    }

    return deleted;
}

// 경로와 이름을 합쳐 전체 경로 생성
static void make_full_path(char* dest, size_t size, const char* dir, const char* name) {
    size_t len = strlen(dir);
    if (len > 0 && (dir[len-1] == '/' || dir[len-1] == '\\')) {
        snprintf(dest, size, "%s%s", dir, name);
    } else {
        snprintf(dest, size, "%s%c%s", dir, PATH_SEPARATOR, name);
    }
}

// 폴더 생성 (성공 시 true)
bool create_folder(const char* dir_path, const char* folder_name) {
    char full_path[512];
    make_full_path(full_path, sizeof(full_path), dir_path, folder_name);

    struct _stat st = {0};
    if (_stat(full_path, &st) == 0 && (st.st_mode & _S_IFDIR)) {
        return true;
    }
#ifdef _WIN32
    if (_mkdir(full_path) == 0) {
#else
    if (mkdir(full_path, 0755) == 0) {
#endif
        return true;
    }
    return false;
}

// C 파일 생성 (성공 시 true)
bool create_c_file(const char* dir_path, const char* file_name) {
    char full_path[512];
    make_full_path(full_path, sizeof(full_path), dir_path, file_name);

    FILE* fp = fopen(full_path, "w");
    if (fp == NULL) return false;
    // 기본 C 파일 템플릿 작성 (선택 사항)
    fprintf(fp, "#include <stdio.h>\n\nint main() {\n    // TODO: implement\n    return 0;\n}\n");
    fclose(fp);
    return true;
}

// 텍스트 파일 생성 (성공 시 true)
bool create_text_file(const char* dir_path, const char* file_name) {
    char full_path[512];
    make_full_path(full_path, sizeof(full_path), dir_path, file_name);

    FILE* fp = fopen(full_path, "w");
    if (fp == NULL) return false;
    fclose(fp);
    return true;
}

// C 코드 파일을 컴파일하고 실행하여 출력값을 result에 저장 (성공 시 true)
bool compile_and_run_c(const char* c_file_path, char* result, int result_size) {
    char exe_path[256];
    char cmd[512];
    char output_file[256];
    FILE* fp;
    bool success = false;

    // 실행 파일 경로 및 출력 파일 경로 설정
    snprintf(exe_path, sizeof(exe_path), "temp_exe.exe");
    snprintf(output_file, sizeof(output_file), "temp_output.txt");

    // 컴파일 명령어 생성 (Windows 기준, gcc 사용)
    snprintf(cmd, sizeof(cmd), "gcc \"%s\" -o \"%s\" 2> compile_error.txt", c_file_path, exe_path);
    if (system(cmd) != 0) {
        // 컴파일 실패
        FILE* err = fopen("compile_error.txt", "r");
        if (err) {
            fread(result, 1, result_size - 1, err);
            result[result_size - 1] = '\0';
            fclose(err);
        } else {
            strncpy(result, "Compile error (no details)", result_size);
        }
        remove("compile_error.txt");
        return false;
    }
    remove("compile_error.txt");

    // 실행 명령어 생성 (출력 리다이렉션)
    snprintf(cmd, sizeof(cmd), "\"%s\" > \"%s\"", exe_path, output_file);
    if (system(cmd) == 0) {
        fp = fopen(output_file, "r");
        if (fp) {
            size_t len = fread(result, 1, result_size - 1, fp);
            result[len] = '\0';
            fclose(fp);
            success = true;
        }
    }
    // 임시 파일 정리
    remove(exe_path);
    remove(output_file);

    return success;
}

// 현재 시간을 "YYYYMMDD_HHMMSS" 문자열로 반환 (result에 저장, 성공 시 true)
bool get_current_time(char* result, int result_size) {
    time_t t = time(NULL);
    struct tm* tm_info = localtime(&t);
    if (!tm_info) return false;
    strftime(result, result_size, "%Y%m%d_%H%M%S", tm_info);
    return true;
}

