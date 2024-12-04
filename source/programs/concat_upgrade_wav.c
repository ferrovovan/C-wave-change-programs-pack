#define BUFF_SIZE 1024
#define PRINT_HEADER 0

#include "FileManager.h"
#include "read_wav.h"

uintptr_t ptr_start, ptr_start_mix, ptr_end_mix, ptr_end_start;
uint32_t beginning_bytes, mix_bytes, end_bytes, result_bytes;


void calculate_constans(WavHeader *header1, WavHeader *header2, double mix_dlit)
{
	// common values
	mix_bytes       = seconds_to_bytes_count(mix_dlit, header1);
	beginning_bytes = calc_main_data_size(header1) - mix_bytes;
	end_bytes       = calc_main_data_size(header2) - mix_bytes;
	result_bytes = (beginning_bytes + mix_bytes + end_bytes);
	// first file
	ptr_start     = (uintptr_t)calc_header_size(header1);
	ptr_start_mix = (uintptr_t)beginning_bytes;
	// second file
	ptr_end_mix   = (uintptr_t)calc_header_size(header2);
	ptr_end_start = ptr_end_mix + (uintptr_t)mix_bytes;
}

WavHeader createCombinedHeader(WavHeader *base_header) {
	WavHeader combinedHeader = *base_header;

	// Изменяем Chunk Size на "сумму размеров обоих заголовков - наложенная длительность"
	combinedHeader.chunkSize = result_bytes + 44;

	strncpy(combinedHeader.subchunk2Id, "data", 4);
	combinedHeader.subchunk2Size = result_bytes;

	return combinedHeader;
}

void concat_wav_files(FILE *inputFile1, FILE *inputFile2, FILE *outputFile, double mix_dlit){
	// Чтение заголовка первого файла
	WavHeader header1;
	readWavHeader(inputFile1, &header1);

	// Чтение  заголовка второго файла
	WavHeader header2;
	readWavHeader(inputFile2, &header2);

	calculate_constans(&header1, &header2, mix_dlit);
	// Создание и Запись итогового заголовка
	WavHeader combinedHeader = createCombinedHeader(&header1);
	fwrite(&combinedHeader, sizeof(WavHeader), 1, outputFile);


	/* Блок записи основных данных */

	uint8_t *buffer;
	// начало чтение + запись
	fseek(inputFile1,  ptr_start , SEEK_SET);
	buffer = (uint8_t *)malloc(beginning_bytes);
	fread(buffer, beginning_bytes, sizeof(uint8_t), inputFile1);
	fwrite(buffer, beginning_bytes, sizeof(uint8_t), outputFile);
	free(buffer);

	/* mix */
	uint8_t *start_buffer, *end_buffer, *mix_buffer;
	// mix first
	start_buffer = (uint8_t *)malloc(mix_bytes);
	fseek(inputFile1,  ptr_start_mix , SEEK_SET);
	fread(start_buffer, sizeof(uint8_t), mix_bytes, inputFile2);
	// mix second
	end_buffer = (uint8_t *)malloc(mix_bytes);
	fseek(inputFile2,  ptr_end_mix , SEEK_SET);
	fread(end_buffer  , sizeof(uint8_t), mix_bytes, inputFile1);
	// result mix
	mix_buffer = (uint8_t *)malloc(mix_bytes);

	if ( (header1.bitsPerSample == 16) &
	     (header2.bitsPerSample == 16) ) {
	float progress;
	int volume_i;
	int16_t start_num=0, end_num=0, result;

	for (uint32_t i = 0; i < mix_bytes; i+=2) {
		start_num = ((int16_t)start_buffer[i+1] << 8) | start_buffer[i];
		end_num   = ((int16_t)end_buffer[i+1]   << 8) | end_buffer[i];

		progress = (float)i / mix_bytes;
		volume_i = (progress * 256 + 0.5);
		start_num = ((int64_t)start_num * volume_i + 128) >> 8;

		progress = 1 - progress;
		volume_i = (progress * 256 + 0.5);
		end_num = ((int64_t)end_num * volume_i + 128) >> 8;


		result = ((int64_t)start_num + (int64_t)end_num);
		mix_buffer[i+1]   = result >> 8;
		mix_buffer[i] = result;
	}
	} else{
		printf("Не поддерживаемая разрядность семплов.\n");
	}

	// запись повтора
	fwrite(mix_buffer, 1, mix_bytes, outputFile);
	free(mix_buffer);

	// запись второго
	fseek(inputFile2,  ptr_end_start , SEEK_SET);
	buffer = (uint8_t *)malloc(end_bytes);
	fread(buffer, end_bytes, sizeof(uint8_t), inputFile2);
	fwrite(buffer, end_bytes, sizeof(uint8_t), outputFile);
	free(buffer);
	/* Конец блока */


	printf("\nФайлы успешно объединены.\n");
}

int main(int argc, char *argv[]) {
	if (argc != 6) {
		printf("Использование: %s <входной_файл1> <входной_файл2> <выходной_файл> -md <длительность наложения>\n", argv[0]);
		return EXIT_FAILURE;
	}

	// Открытие файлов
	FileManager fm;  init_FileManager(&fm);

	FILE* inputFile1 = safe_open_file(&fm, argv[1], "rb");
	FILE* inputFile2 = safe_open_file(&fm, argv[2], "rb");
	FILE* outputFile = safe_open_file(&fm, argv[3], "wb");

	double mix_dlit = HHMMSS_to_seconds(argv[5]);
	concat_wav_files(inputFile1, inputFile2, outputFile, mix_dlit);

	close_all_files(&fm);
	return EXIT_SUCCESS;
}



