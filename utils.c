// utils.c
#include "utils.h"
#include <string.h>
#include <stdio.h>
#include <stdbool.h>
#include <sys/stat.h>
#include <time.h>
#include <stdlib.h>

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

// ���� ���� (���� �� true)
bool create_folder(const char* path) {
    // �̹� �����ϸ� ���� ó��
    struct _stat st = {0};
    if (_stat(path, &st) == 0 && (st.st_mode & _S_IFDIR)) {
        return true;
    }
    // ���� ����
    if (_mkdir(path) == 0) {
        return true;
    }
    return false;
}

// C ���� ���� (���� �� true)
bool create_c_file(const char* path) {
    FILE* fp = fopen(path, "w");
    if (fp == NULL) return false;
    // �⺻ C ���� ���ø� �ۼ� (���� ����)
    fprintf(fp, "#include <stdio.h>\n\nint main() {\n    // TODO: implement\n    return 0;\n}\n");
    fclose(fp);
    return true;
}

// �ؽ�Ʈ ���� ���� (���� �� true)
bool create_text_file(const char* path) {
    FILE* fp = fopen(path, "w");
    if (fp == NULL) return false;
    fclose(fp);
    return true;
}

// C �ڵ� ������ �������ϰ� �����Ͽ� ��°��� result�� ���� (���� �� true)
bool compile_and_run_c(const char* c_file_path, char* result, int result_size) {
    char exe_path[256];
    char cmd[512];
    char output_file[256];
    FILE* fp;
    bool success = false;

    // ���� ���� ��� �� ��� ���� ��� ����
    snprintf(exe_path, sizeof(exe_path), "temp_exe.exe");
    snprintf(output_file, sizeof(output_file), "temp_output.txt");

    // ������ ��ɾ� ���� (Windows ����, gcc ���)
    snprintf(cmd, sizeof(cmd), "gcc \"%s\" -o \"%s\" 2> compile_error.txt", c_file_path, exe_path);
    if (system(cmd) != 0) {
        // ������ ����
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

    // ���� ��ɾ� ���� (��� �����̷���)
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
    // �ӽ� ���� ����
    remove(exe_path);
    remove(output_file);

    return success;
}

// ���� �ð��� "YYYYMMDD_HHMMSS" ���ڿ��� ��ȯ (result�� ����, ���� �� true)
bool get_current_time(char* result, int result_size) {
    time_t t = time(NULL);
    struct tm* tm_info = localtime(&t);
    if (!tm_info) return false;
    strftime(result, result_size, "%Y%m%d_%H%M%S", tm_info);
    return true;
}