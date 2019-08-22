BUILD := DEBUG
SAN := address # thread,memory,undefined
ifeq ($(BUILD),RELEASE)
  CFLAGS := -O3 -Wall -Wextra
else ifeq ($(BUILD),DEBUG)
  CFLAGS := -fsanitize=$(SAN) -g3 -Wall -Wextra
else
  $(error Usage: make BUILD=[RELEASE,DEBUG])
endif

CC := clang

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