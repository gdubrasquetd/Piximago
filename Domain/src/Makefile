CC = gcc
CFLAGS = -Wall -O2 -Wno-unused-variable

LIBS = -limago -lm -lpng -ljpeg
LIBS_FFT = -limago -lfftw3 -lpng -ljpeg -lm -ldl

BUILD_DIR = ../../build

all: mkdir_build band balance colors convolution cross stain resize

mkdir_build:
	mkdir -p $(BUILD_DIR)

band: band.c fft.c utils.c
	$(CC) $(CFLAGS) $^ $(LIBS_FFT) -o $(BUILD_DIR)/band

balance: balance.c utils.c
	$(CC) $(CFLAGS) $^ $(LIBS) -o $(BUILD_DIR)/balance

colors: colors.c utils.c
	$(CC) $(CFLAGS) $^ $(LIBS) -o $(BUILD_DIR)/colors

convolution: convolution.c fft.c utils.c
	$(CC) $(CFLAGS) $^ $(LIBS_FFT) -o $(BUILD_DIR)/convolution

cross: cross.c fft.c utils.c
	$(CC) $(CFLAGS) $^ $(LIBS_FFT) -o $(BUILD_DIR)/cross

stain: stain.c fft.c utils.c
	$(CC) $(CFLAGS) $^ $(LIBS_FFT) -o $(BUILD_DIR)/stain

resize: resize.c utils.c
	$(CC) $(CFLAGS) $^ $(LIBS_FFT) -o $(BUILD_DIR)/resize

clean:
	rm -f $(BUILD_DIR)/band $(BUILD_DIR)/balance $(BUILD_DIR)/colors $(BUILD_DIR)/convolution $(BUILD_DIR)/cross $(BUILD_DIR)/stain $(BUILD_DIR)/resize
