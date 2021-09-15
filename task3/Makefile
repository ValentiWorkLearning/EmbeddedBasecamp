
BIN=./bin/
SOURCE=./src/

CC = gcc
CFLAGS =  -g -Wall

.PHONY: all

all: pack upack

pack: src/encoder.c
	$(CC) $(CFLAGS) -o bin/pack src/encoder.c

upack: src/decoder.c
	$(CC) $(CFLAGS) -o bin/upack src/decoder.c

clean:
	$(RM) -rf *.o
