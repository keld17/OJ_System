// utils.c
#include "utils.h"
#include <string.h>

// ���Ͽ��� �� �� �б�
bool read_line(FILE* fp, char* buffer, int buffer_size) {
    if (fgets(buffer, buffer_size, fp) == NULL)
        return false;
    // ���� ���� ����
    buffer[strcspn(buffer, "\r\n")] = '\0';
    return true;
}

// ���Ͽ� �� �� ����
bool write_line(FILE* fp, const char* line) {
    if (fprintf(fp, "%s\n", line) < 0)
        return false;
    return true;
}

// ���Ͽ��� Ư�� �� ����
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