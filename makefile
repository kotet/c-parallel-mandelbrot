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

all: serial thread

serial: main.o mandel/base.o mandel/serial.o
	$(CC) $(CFLAGS) -o $@ $^

thread: main.o mandel/base.o mandel/thread.o
	$(CC) $(CFLAGS) -lpthread -o $@ $^

clean:
	rm -fv *.o
	rm -fv *.pgm
	rm -fv mandel/*.o
	rm -fv serial
	rm -fv thread

%.o: %.c
	$(CC) $(CFLAGS) -c -o $@ $<