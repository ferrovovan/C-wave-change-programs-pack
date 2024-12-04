// args parser including
#define REQ_ARG_INPUT
#define REQ_ARG_OUTPUT
#define REQ_ARG_COUNT

#include "getopt_args_parser.h"
// including end

#include "FileManager.h"
#include "read_wav.h"

#define BUFF_SIZE 1024
#define PRINT_HEADER 1


void mult_wave_file(FILE *inputFile, FILE *outputFile, const int cycles){
	// Считывание заголовка первого файла
	WavHeader in_header;
	readWavHeader(inputFile, &in_header);
	if (PRINT_HEADER){
		printf("\nIn header:\n");
		printWavHeader(&in_header);
	}

    
	// Создание нового заголовка для выходного файла
	WavHeader out_header;
	copy_WavHeader(&in_header, &out_header);
	uint32_t result_bytes = calc_main_data_size(&in_header) * cycles;
	out_header.chunkSize = result_bytes + 44;
	out_header.subchunk2Size = result_bytes;

	if (PRINT_HEADER){
		printf("\nOut header:\n");
		printWavHeader(&out_header);
	}
	fwrite(&out_header, sizeof(out_header), 1, outputFile);    


	uint8_t buffer[BUFF_SIZE];
	size_t bytesRead;
	size_t samples_start = calc_header_size(&in_header);
	
	for (int i=0; i<cycles; i++){
		fseek(inputFile, samples_start, SEEK_SET);
		while ((bytesRead = fread(buffer, 1, sizeof(buffer), inputFile)) > 0)
			fwrite(buffer, sizeof(uint8_t), bytesRead, outputFile);
	}

	printf("\nФайл успешно умножен.\n");
}

int main(int argc, char *argv[]) {
	// Считывание
	if (parse_arguments(argc, argv) != 0) {
		return EXIT_FAILURE;
	}
	
	// Открытие файлов
	FileManager fm;  init_FileManager(&fm);

	FILE* inputFile  = safe_open_file(&fm, input_file, "rb");
	FILE* outputFile = safe_open_file(&fm, output_file, "wb");)

// Валидация
	int cycles = atoi(count);
	if( cycles == 0 ){
		printf("Некорректное значение циклов.\n");
		return EXIT_FAILURE;
	}

	// Передача аргументов в функцию
	mult_wave_file(inputFile, outputFile, cycles);

	close_all_files(&fm);
	return EXIT_SUCCESS;
}

