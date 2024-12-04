#include "read_wav.h"

int validate_cut_times(WavHeader *header, double start_time, double end_time,
                    uint32_t *start_bytes, uint32_t *end_bytes, uint32_t *cut_bytes) {
	*start_bytes = seconds_to_bytes_count(start_time, header);
	*end_bytes = seconds_to_bytes_count(end_time, header);

	if (*end_bytes <= *start_bytes) {
		fprintf(stderr, "Ошибка: конец обрезки меньше или равен началу.\n");
		return EXIT_FAILURE;
	}

	*cut_bytes = *end_bytes - *start_bytes;
	return 0;
}

void print_cut_info(WavHeader *header,
                    uint32_t start_bytes, uint32_t end_bytes, uint32_t cut_bytes) {
	printf("\nПервоначальный заголовок:\n");
	printWavHeader(header);

	printf("\n-----------\n");
	printf("Байтовый блок:\n");
	printf("Начало: %u байт;\n", start_bytes);
	printf("Конец: %u байт;\n", end_bytes);

	printf("\nTime block:\n");
	double duration = bytes_count_to_seconds(cut_bytes, header);
	printf("\nДлительность вырезанного блока: %.2f секунд;\n", duration);
}

int process_cut(FILE *inputFile, FILE *outputFile, WavHeader *in_header,
                    uint32_t start_bytes, uint32_t cut_bytes) {
	WavHeader out_header;
	copy_WavHeader(in_header, &out_header);
	out_header.chunkSize = cut_bytes + sizeof(WavHeader);
	out_header.subchunk2Size = cut_bytes;

	if (fwrite(&out_header, sizeof(out_header), 1, outputFile) != 1) {
		perror("Ошибка записи заголовка.");
		return EXIT_FAILURE;
	}

	uint8_t *buffer = (uint8_t *)malloc(cut_bytes);
	if (!buffer) {
		perror("Ошибка выделения памяти.");
		return EXIT_FAILURE;
	}

	fseek(inputFile, start_bytes + calc_header_size(in_header), SEEK_SET);
	if (fread(buffer, cut_bytes, 1, inputFile) != 1) {
		perror("Ошибка чтения данных.");
		free(buffer);
		return EXIT_FAILURE;
	}

	if (fwrite(buffer, cut_bytes, 1, outputFile) != 1) {
		perror("Ошибка записи данных.");
		free(buffer);
		return EXIT_FAILURE;
	}

	free(buffer);
	printf("\nУспешная запись файла.\n");
	return 0;
}

int cut_wav_file(FILE *inputFile, FILE *outputFile, double start_time, double end_time, short info) {
	WavHeader in_header;
	if (readWavHeader(inputFile, &in_header) != 0) {
		fprintf(stderr, "Ошибка чтения заголовка WAV.\n");
		return EXIT_FAILURE;
	}

	// Валидация времени и расчёт байтов
	uint32_t start_bytes, end_bytes, cut_bytes;
	if (validate_cut_times(&in_header, start_time, end_time,
                               &start_bytes, &end_bytes, &cut_bytes) != 0) {
		return EXIT_FAILURE;
	}

	if (info) {
		print_cut_info(&in_header, start_bytes, end_bytes, cut_bytes);
		return 0;
	}

	// Обработка и запись файла
	return process_cut(inputFile, outputFile, &in_header, start_bytes, cut_bytes);
}

