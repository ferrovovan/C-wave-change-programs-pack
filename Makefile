CC = gcc
CFLAGS = -Wall -Wextra -std=c99 -o2

SRCDIR = source/programs
BINDIR = build

OS := $(shell uname)

postfix = 

ifeq ($(OS), Linux)
    # Команды для Linux
endif

ifeq ($(OS), Darwin)
    # Команды для macOS
endif

ifeq ($(OS), Windows_NT)
	postfix = .exe
endif


INCLUDE_DIRS = -Isource/libraries
HOUR_WAV_DIR = $(SRCDIR)/1_hour_wav/


$(BINDIR):
	mkdir  $(BINDIR)

wav_info: $(BINDIR)
	$(CC) $(CFLAGS) $(INCLUDE_DIRS) -o $(BINDIR)/wav_info$(postfix) $(SRCDIR)/wav_info.c

#boosted_wav:
#	$(CC) $(CFLAGS) -o $(BINDIR)/boosted_wav$(postfix) $(SRCDIR)/boosted_wav.c

#slowed_wav:
#	$(CC) $(CFLAGS) $(INCLUDE_DIRS) -o $(BINDIR)/slowed_wav$(postfix) $(SRCDIR)/slowed_wav.c

concat_wav: $(BINDIR)
	$(CC) $(CFLAGS) $(INCLUDE_DIRS) -o $(BINDIR)/concat_wav$(postfix) $(SRCDIR)/concat_wav.c

concat_upgrade_wav: $(BINDIR)
	$(CC) $(CFLAGS) $(INCLUDE_DIRS) -o $(BINDIR)/concat_upgrade_wav$(postfix) $(SRCDIR)/concat_upgrade_wav.c

mult_wav: $(BINDIR)
	$(CC) $(CFLAGS) $(INCLUDE_DIRS) -o $(BINDIR)/mult_wav$(postfix) $(SRCDIR)/mult_wav.c

cut_in_wav: $(BINDIR)
	$(CC) $(CFLAGS) $(INCLUDE_DIRS) -o $(BINDIR)/cut_in_wav$(postfix) $(SRCDIR)/cut_in_wav.c

cut_out_wav: $(BINDIR)
	$(CC) $(CFLAGS) $(INCLUDE_DIRS) -o $(BINDIR)/cut_out_wav$(postfix) $(SRCDIR)/cut_out_wav.c

insert_wav:
	$(CC) $(CFLAGS) $(INCLUDE_DIRS) -o $(BINDIR)/insert_wav$(postfix) $(SRCDIR)/insert_wav.c

fan_1_hour_wav: $(BINDIR)
	$(CC) $(CFLAGS) $(INCLUDE_DIRS) -o $(BINDIR)/fan_1_hour_wav$(postfix) $(SRCDIR)/fan_1_hour_wav.c

two_channels_wav: $(BINDIR)
	$(CC) $(CFLAGS) $(INCLUDE_DIRS) -o $(BINDIR)/two_channels_wav$(postfix) $(SRCDIR)/two_channels_wav.c

form_pitch_wav: $(BINDIR)
	$(CC) $(CFLAGS) $(INCLUDE_DIRS) -o $(BINDIR)/form_pitch_wav$(postfix) $(SRCDIR)/form_pitch_wav.c


SINGLE_TARGETS := mult_wav concat_wav wav_info cut_in_wav cut_out_wav insert_wav fan_1_hour_wav two_channels_wav zero_wav null_wav form_pitch_wav
# boosted_wav slowed_wav


1_hour_wav: $(BINDIR)
	$(MAKE) -C $(HOUR_WAV_DIR)
	mv $(HOUR_WAV_DIR)1_hour_extender$(postfix)  $(BINDIR)/1_hour_extender$(postfix)

zero_wav: $(BINDIR)
	$(CC) $(CFLAGS) $(INCLUDE_DIRS) -o $(BINDIR)/zero_wav$(postfix) $(SRCDIR)/null_wav.c

null_wav: $(BINDIR)
	$(CC) $(CFLAGS) $(INCLUDE_DIRS) -o $(BINDIR)/zero_wav$(postfix) $(SRCDIR)/null_wav.c

all: $(SINGLE_TARGETS) 1_hour_wav

clear:
	rm -rf $(BINDIR)
