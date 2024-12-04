// args parser including
#define REQ_ARG_INPUT
#define REQ_ARG_OUTPUT

#include "getopt_args_parser.h"
// including end


#define BUFF_SIZE 1024
#define PRINT_HEADER 1

#include "FileManager.h"
#include "read_wav.h"


void bisect_wav(FILE *inputFile, FILE *outputFile){
	WavHeader in_header;
	readWavHeader(inputFile, &in_header);
	if (PRINT_HEADER){
		printf("\nfirst header:\n");
		printWavHeader(&in_header);
	}
	if (in_header.numChannels == 2){
		printf("File alredy have 2 channels\n");
		return EXIT_FAILURE;
	}

	// Создание нового заголовка для выходного файла
	WavHeader outHeader;
	create_WavHeader_base(&outHeader, 2);
	uint32_t result_bytes = (in_header.chunkSize - calc_header_size(&in_header)) * 2;
	outHeader.chunkSize = result_bytes + 44;
	outHeader.subchunk2Size = result_bytes;
	fwrite(&outHeader, sizeof(WavHeader), 1, outputFile);    

	// Копирование данных из второго файла в конец первого
	uint8_t buffer[BUFF_SIZE];
	uint8_t buffer2[2* BUFF_SIZE];
	size_t bytesRead;


	while ((bytesRead = fread(buffer, 1, sizeof(buffer), inputFile)) > 0){
		for(size_t i=0; i< bytesRead; ++i){
			buffer2[i*2]     = buffer[i];
			buffer2[i*2 + 1] = buffer[i];
		}
		fwrite(buffer2, 1, 2*bytesRead, outputFile);
	}

	printf("\nАудио успешно дополнено до стерео вида.\n");
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

	bisect_wav(inputFile, outputFile);

	close_all_files(&fm);
	return EXIT_SUCCESS;
}

