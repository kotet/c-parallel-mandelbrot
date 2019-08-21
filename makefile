# CFLAGS := -O3 -Wall -Wextra
CFLAGS := -fsanitize=address -Wall -Wextra
CC := gcc

all: serial

serial: main.o mandel/base.o mandel/serial.o
	$(CC) $(CFLAGS) -o $@ $^

clean:
	rm -fv *.o
	rm -fv *.pgm
	rm -fv mandel/*.o
	rm -fv serial

%.o: %.c
	$(CC) $(CFLAGS) -c -o $@ $<