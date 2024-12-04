#include "FileManager.h"
#include "read_wav.h"


int main(int argc, char *argv[]) {
	if (argc != 2) {
		printf("Использование: %s <входной_файл>\n", argv[0]);
		return 1;
	}
	
	// Открытие файлов
	FileManager fm;  init_FileManager(&fm);

	FILE* inputFile  = safe_open_file(&fm, input_file, "rb");
	
	WavHeader header;
	readWavHeader(inputFile, &header);
	printf("\nheader\n");
	printWavHeader(&header);

	close_all_files(&fm);
	return EXIT_SUCCESS;
}


