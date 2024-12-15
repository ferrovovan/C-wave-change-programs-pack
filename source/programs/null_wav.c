// args parser including
#define REQ_ARG_OUTPUT
#define REQ_ARG_ED_TIME

#include "getopt_args_parser.h"
// including end


#include "FileManager.h"
#include "read_wav.h"
#include <stdint.h>

#define BUFF_SIZE 1024


void zero_wav_file(FILE *outputFile, double time){
    
	// Создание нового заголовка для выходного файла
	WavHeader outHeader;
	create_WavHeader_base(&outHeader, 2);
	uint32_t result_bytes = seconds_to_bytes_count(time, &outHeader);
	outHeader.chunkSize = result_bytes + 44;
	outHeader.subchunk2Size = result_bytes;

	// Запись нового заголовка в выходной файл
	fwrite(&outHeader, sizeof(WavHeader), 1, outputFile);    


	const uint8_t *buffer = (uint8_t *) calloc(BUFF_SIZE, sizeof(uint8_t) );
	const int counts = result_bytes / BUFF_SIZE;
	const int remainder = result_bytes % BUFF_SIZE;
	for (int i = 0; i < counts; ++i)
		fwrite(buffer, sizeof(uint8_t), BUFF_SIZE, outputFile);
	fwrite(buffer, sizeof(uint8_t), remainder, outputFile);

	printf("\nФайл успешно занулён.\n");
}



int main(int argc, char *argv[]) {
	// Считывание
	if (parse_arguments(argc, argv) != 0) {
		return EXIT_FAILURE;
	}
	
	// Открытие файлов
	FileManager fm;  init_FileManager(&fm);

	FILE* outputFile = safe_open_file(&fm, output_file, "wb");

	// Открытие параметров
	double time = validate_and_convert_time(expected_duration, -1, "expected_duration");
	if (time == -1) return EXIT_FAILURE;

	// Передача аргументов в функцию
	zero_wav_file(outputFile, time);

	close_all_files(&fm);
	return EXIT_SUCCESS;
}

