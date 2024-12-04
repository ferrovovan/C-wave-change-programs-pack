#define BUFF_SIZE 1024
#define PRINT_HEADER 0

#include "FileManager.h"
#include "read_wav.h"


WavHeader createCombinedHeader(WavHeader *header1, WavHeader *header2) {
	WavHeader combinedHeader = *header1;  // Копируем все поля из первого заголовка

	// Изменяем Chunk Size на сумму Chunk Size из обоих заголовков
	uint32_t first_samples_size	= header1->chunkSize - calc_header_size(header1);
	uint32_t second_samples_size	= header2->chunkSize - calc_header_size(header2);

	uint32_t samples_size = first_samples_size + second_samples_size;
	
	combinedHeader.chunkSize = samples_size + 44;
	

	strncpy(combinedHeader.subchunk2Id, "data", 4);
	combinedHeader.subchunk2Size = samples_size;

	return combinedHeader;
}

void concat_wav_files(FILE *inputFile1, FILE *inputFile2, FILE *outputFile){
	// Чтение заголовка первого файла
	WavHeader header1;
	readWavHeader(inputFile1, &header1);
	if (PRINT_HEADER){
		printf("\nfirst header:\n");
		printWavHeader(&header1);
	}

	// Чтение  заголовка второго файла
	WavHeader header2;
	readWavHeader(inputFile2, &header2);
	if (PRINT_HEADER){
		printf("\nsecond header:\n");
		printWavHeader(&header2);
	}
    
	// Создание нового заголовка для выходного файла
	WavHeader combinedHeader = createCombinedHeader(&header1, &header2);
	if (PRINT_HEADER){
		printf("\ncombined Header\n");
		printWavHeader(&combinedHeader);
	}
    
	// Запись нового заголовка в выходной файл
	fwrite(&combinedHeader, sizeof(WavHeader), 1, outputFile);    

	// Копирование данных из второго файла в конец первого
	uint8_t buffer[BUFF_SIZE];
	size_t bytesRead;


	while ((bytesRead = fread(buffer, 1, sizeof(buffer), inputFile1)) > 0)
		fwrite(buffer, 1, bytesRead, outputFile);
	while ((bytesRead = fread(buffer, 1, sizeof(buffer), inputFile2)) > 0)
		fwrite(buffer, 1, bytesRead, outputFile);


	printf("\nФайлы успешно объединены.\n");
}

int main(int argc, char *argv[]) {
	if (argc != 4) {
		printf("Использование: %s <входной_файл1> <входной_файл2> <выходной_файл>\n", argv[0]);
		return EXIT_FAILURE;
	}
	
	// Открытие файлов
	FileManager fm;  init_FileManager(&fm);

	FILE* inputFile1 = safe_open_file(&fm, argv[1], "rb");
	FILE* inputFile2 = safe_open_file(&fm, argv[2], "rb");
	FILE* outputFile = safe_open_file(&fm, argv[3], "wb");

	concat_wav_files(inputFile1, inputFile2, outputFile);
	
	close_all_files(&fm);
	return EXIT_SUCCESS;
}

