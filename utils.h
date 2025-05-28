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

#endif
