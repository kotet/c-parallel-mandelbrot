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

all: serial thread fork_pipe

test: all
	./serial
	./thread
	./fork_pipe
	diff serial.pgm thread.pgm
	diff serial.pgm fork_pipe.pgm

serial: main.o mandel/base.o mandel/serial.o
	$(CC) $(CFLAGS) -o $@ $^

thread: main.o mandel/base.o mandel/thread.o
	$(CC) $(CFLAGS) -lpthread -o $@ $^

fork_pipe: main.o mandel/base.o mandel/fork_pipe.o
	$(CC) $(CFLAGS) -o $@ $^

clean:
	rm -fv *.o
	rm -fv *.pgm
	rm -fv mandel/*.o
	rm -fv serial
	rm -fv thread
	rm -fv fork_pipe

%.o: %.c
	$(CC) $(CFLAGS) -c -o $@ $<