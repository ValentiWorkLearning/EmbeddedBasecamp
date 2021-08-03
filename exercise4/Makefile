CC = gcc
CFLAGS = -Wall -g

TARGET = game

#https://www.cs.swarthmore.edu/~newhall/unixhelp/howto_makefiles.html

all: $(TARGET)


$(TARGET): $(TARGET).c
	$(CC) $(CFLAGS) -o $(TARGET) $(TARGET).c

clean:
	$(RM) $(TARGET)