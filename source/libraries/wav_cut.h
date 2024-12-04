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

int cut_in_wav_file(FILE *inputFile, FILE *outputFile, double start_time, double end_time, short info) {
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

#define CHS calc_header_size(&in_header)

int cut_out_wav_file(FILE *inputFile, FILE *outputFile, double start_time, double end_time, double mix_duration, short info){

	WavHeader in_header;
	readWavHeader(inputFile, &in_header);


	const uintptr_t ptr_start     = CHS;
	const uintptr_t ptr_end       = in_header.chunkSize;
	const uintptr_t ptr_start_cut =  (uintptr_t)seconds_to_bytes_count(start_time, &in_header) + CHS;
	const uintptr_t   ptr_end_cut   = (uintptr_t)seconds_to_bytes_count(end_time, &in_header) + CHS;


	if (ptr_end_cut <= ptr_start_cut){
		perror("Конец меньше начала.\n");
		return -1;
	}

	const uint32_t start_t_bytes_count = ptr_start_cut - CHS;
	const uint32_t end_t_bytes_count   = ptr_end - ptr_end_cut;
	const uint32_t md_bytes_count = seconds_to_bytes_count(mix_duration, &in_header);
	const uint32_t res_duration_bytes = start_t_bytes_count + end_t_bytes_count - md_bytes_count;


	if (info){
		print_cut_info(&in_header, start_t_bytes_count, end_t_bytes_count, res_duration_bytes);
		return 0;
	} else {
		// Создание нового заголовка для выходного файла
		WavHeader cutHeader;
		copy_WavHeader(&in_header, &cutHeader);
		cutHeader.chunkSize = res_duration_bytes + 44;
		cutHeader.subchunk2Size = res_duration_bytes;
		fwrite(&cutHeader, sizeof(cutHeader), 1, outputFile);


		/* алгоритм */
		uint8_t *buffer;
		uint32_t temp = start_t_bytes_count - md_bytes_count;
		buffer = (uint8_t *)malloc(temp);

		fseek(inputFile,  ptr_start , SEEK_SET);
		fread(buffer, temp, sizeof(uint8_t), inputFile);
		fwrite(buffer, temp, sizeof(uint8_t), outputFile);
		free(buffer);


	// запись смешивания
		uint8_t *start_buffer = (uint8_t *)malloc(md_bytes_count);
		fread(start_buffer, sizeof(uint8_t), md_bytes_count, inputFile);

		uint8_t *end_buffer = (uint8_t *)malloc(md_bytes_count);
	fseek(inputFile,  ptr_end_cut , SEEK_SET);
	fread(end_buffer, sizeof(uint8_t), md_bytes_count, inputFile);


		uint8_t *mix_buffer = (uint8_t *)malloc(md_bytes_count);
		if (in_header.bitsPerSample == 16)
	{
	int16_t start_num=0, end_num=0, result, volume_i;
	float progress;

	for (uint32_t i = 0; i < md_bytes_count; i+=2) {
		start_num = ((int16_t)start_buffer[i+1] << 8) | start_buffer[i];
		end_num   = ((int16_t)end_buffer[i+1]   << 8) | end_buffer[i];

		progress = (float)i / md_bytes_count;
		volume_i = (progress * 256 + 0.5);
		start_num = ((int64_t)start_num * volume_i + 128) >> 8;

		progress = 1 - progress;
		volume_i = (progress * 256 + 0.5);
		end_num = ((int64_t)end_num * volume_i + 128) >> 8;


		result = ((int64_t)start_num + (int64_t)end_num);
		mix_buffer[i+1]   = result >> 8;
		mix_buffer[i] = result;
	}
		}else{
			printf("Не поддерживаемая разрядность семплов: %d (bitsPerSample).\n", in_header.bitsPerSample);
			return -1;
		}

		fwrite(mix_buffer, md_bytes_count, sizeof(uint8_t), outputFile);
		free(mix_buffer);


		temp = end_t_bytes_count - md_bytes_count;
		buffer = (uint8_t *)malloc(temp);
		fread(buffer, temp, sizeof(uint8_t), inputFile);
		fwrite(buffer, temp, sizeof(uint8_t), outputFile);


		fwrite(buffer, res_duration_bytes, sizeof(uint8_t), outputFile);
		free(buffer);

		printf("\nУспешная запись.\n");
		return 0;
	}
}
