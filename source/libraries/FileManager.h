#include <stdlib.h>
#include <stdio.h>

#define MAX_FILES 16

typedef struct {
	FILE* files[MAX_FILES];
	size_t count;
} FileManager;

// Инициализация менеджера
void init_FileManager(FileManager* fm) {
	fm->count = 0;
}

void add_file(FileManager* fm, FILE* file) {
	if (fm->count < MAX_FILES) {
		fm->files[fm->count++] = file;
	}
}

void close_all_files(FileManager* fm) {
	for (size_t i = 0; i < fm->count; ++i) {
		fclose(fm->files[i]);
	}
	fm->count = 0;
}

void report_file_error(const char* filename, const char* mode) {
	fprintf(stderr, "Ошибка открытия файла '%s' в режиме '%s'.\n", filename, mode);
}

FILE* safe_open_file(FileManager* fm, const char* filename, const char* mode) {
	FILE* file = fopen(filename, mode);
	if (file == NULL) {
		report_file_error(filename, mode);
		close_all_files(fm);
		exit(EXIT_FAILURE);
	}
	add_file(fm, file);
	return file;
}
