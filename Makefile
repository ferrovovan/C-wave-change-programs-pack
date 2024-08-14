CC = gcc
CFLAGS = -Wall -Wextra -std=c99

SRCDIR = source
BINDIR = bin


concat_wav: $(SRCDIR)/concat_wav.c
	$(CC) $(CFLAGS) -o concat_wav $(SRCDIR)/concat_wav.c

mult_wav: $(SRCDIR)/mult_wav.c
	$(CC) $(CFLAGS) -o mult_wav $(SRCDIR)/mult_wav.c

slowed_wav: $(SRCDIR)/slowed_wav.c
	$(CC) $(CFLAGS) -o slowed_wav $(SRCDIR)/slowed_wav.c

boosted_wav: $(SRCDIR)/boosted_wav.c
	$(CC) $(CFLAGS) -o boosted_wav $(SRCDIR)/boosted_wav.c

wav_info: $(SRCDIR)/wav_info.c
	$(CC) $(CFLAGS) -o wav_info $(SRCDIR)/wav_info.c

