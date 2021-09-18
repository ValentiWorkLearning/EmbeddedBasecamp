#https://makefiletutorial.com/#makefile-cookbook
BUILD_DIR=./bin/
SOURCE=./src/

CC = gcc
CFLAGS =  -g -Wall

.PHONY: all

all: pack upack

pack: src/encoder.c
	$(CC) $(CFLAGS) -o $(BUILD_DIR)/pack src/encoder.c

upack: src/decoder.c
	$(CC) $(CFLAGS) -o $(BUILD_DIR)/upack src/decoder.c

clean:
	$(RM) -rf *.o
	$(RM) -rf $(BUILD_DIR)/rle_encoder_valenti
install: pack upack
	- mkdir $(BUILD_DIR)rle_encoder_valenti
	- mkdir $(BUILD_DIR)rle_encoder_valenti/DEBIAN
	- cp $(SOURCE)control $(BUILD_DIR)rle_encoder_valenti/DEBIAN/control
	- mkdir $(BUILD_DIR)rle_encoder_valenti/opt
	- mkdir $(BUILD_DIR)rle_encoder_valenti/opt/valenti_work_rle
	yes | cp -rf $(BUILD_DIR)pack $(BUILD_DIR)rle_encoder_valenti/opt/valenti_work_rle
	yes | cp -rf $(BUILD_DIR)upack $(BUILD_DIR)rle_encoder_valenti/opt/valenti_work_rle
	dpkg-deb --build $(BUILD_DIR)rle_encoder_valenti
	mv $(BUILD_DIR)rle_encoder_valenti.deb $(BUILD_DIR)rle_encoder_valenti-1.0_amd64.deb
