// utils.c
#include "utils.h"
#include <string.h>

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