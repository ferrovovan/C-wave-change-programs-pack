#define BUFF_SIZE 1024
#define PRINT_HEADER 1

#include "FileManager.h"
#include "read_wav.h"


WavHeader create_extendedHeader(WavHeader *in_header, const int num) {
	WavHeader extendedHeader = *in_header;  // Копируем все поля из первого заголовка

	// Изменяем Chunk Size на сумму Chunk Size из обоих заголовков
	uint32_t samples_size	= in_header->chunkSize - calc_header_size(in_header);
	samples_size = samples_size * num;

	extendedHeader.chunkSize = samples_size + 44;

	strcpy(extendedHeader.subchunk2Id, "data");
	extendedHeader.subchunk2Size = samples_size;

	return extendedHeader;
}


void extend_wave_file(FILE *inputFile, FILE *outputFile, int slowdownFactor){
	// Чтение заголовка первого файла
	WavHeader in_header;
	readWavHeader(inputFile, &in_header);
	if (PRINT_HEADER){
		printf("\nIn header:\n");
		printWavHeader(&in_header);
	}

	// Создание нового заголовка для выходного файла
	WavHeader out_header = create_extendedHeader(&in_header, slowdownFactor);
	if (PRINT_HEADER){
		printf("\nOut header:\n");
		printWavHeader(&out_header);
	}
	
	// Запись нового заголовка в выходной файл
	fwrite(&out_header, sizeof(out_header), 1, outputFile);

	// Размер блока аудио (в байтах) определяется по размеру данных в заголовке WAV файла
	uint32_t blockSize = in_header.numChannels * (in_header.bitsPerSample / 8);
	// Определяем количество сэмплов в блоке
	uint32_t numSamples = out_header.subchunk2Size / (blockSize * slowdownFactor);
	uint8_t sampleData[blockSize];
	printf("blockSize = %d\n ", blockSize);


	// Записываем данные блок за блоком с увеличенным промежутком между блоками
	for (uint32_t sampleNum = 0; sampleNum < numSamples; sampleNum++) {

		fread (sampleData, sizeof(uint8_t), blockSize, inputFile);

		for (int j = 0; j < slowdownFactor; j++)
			fwrite(sampleData, sizeof(uint8_t), blockSize, outputFile);
	}

	printf("\nФайл успешно замедлен.\n");
}

int main(int argc, char *argv[]) {
	if (argc != 4) {
		printf("Использование: %s <входной_файл> <выходной_файл> <коеффициент замедления>\n", argv[0]);
		return 1;
	}
	// Открытие файлов
	FileManager fm;  init_FileManager(&fm);

	FILE* inputFile  = safe_open_file(&fm, argv[1], "rb");
	FILE* outputFile = safe_open_file(&fm, argv[2], "wb");

	int number = atoi(argv[3]);	// Преобразование строки в целое число
	// Передача аргументов в функцию
	extend_wave_file(inputFile, outputFile, number);

	close_all_files(&fm);
	return EXIT_SUCCESS;
}

