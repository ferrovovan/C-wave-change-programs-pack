// args parser including
#define REQ_ARG_INPUT
#define REQ_ARG_OUTPUT

#define ARG_START_TIME
#define ARG_END_TIME
#define ARG_MD_TIME

#define ARG_TEST_FLAG

#include "getopt_args_parser.h"
// including end

#include "FileManager.h"
#include "wav_cut.h"


int main(int argc, char *argv[]) {
	// Считывание
	if (parse_arguments(argc, argv) != 0) {
		return EXIT_FAILURE;
	}

	// Открытие файлов
	FileManager fm;  init_FileManager(&fm);

	FILE* inputFile  = safe_open_file(&fm, input_file, "rb");
	FILE* outputFile = safe_open_file(&fm, output_file, "wb");

	// Открытие параметров
	double start_time_double = validate_and_convert_time(start_time, 0, "start_time");
	if (start_time_double == -1) return EXIT_FAILURE;

	double end_time_double = validate_and_convert_time(end_time, -1, "end_time");
	if (end_time_double == -1) return EXIT_FAILURE;

	double mix_duration_double = validate_and_convert_time(mix_duration, -0, "mix_duration");
	if (end_time_double == -1) return EXIT_FAILURE;

	// Передача аргументов в функцию
	cut_out_wav_file(inputFile, outputFile,
                         start_time_double, end_time_double, mix_duration_double, (short )test_flag);

	close_all_files(&fm);
	return EXIT_SUCCESS;
}


