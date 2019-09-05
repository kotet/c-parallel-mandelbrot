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

TARGETS = serial thread fork_pipe thread_scheduling_mutex

all: $(TARGETS)

test: all
	./serial
	./thread
	./fork_pipe
	./thread_scheduling_mutex
	@diff -s serial.pgm thread.pgm
	@diff -s serial.pgm fork_pipe.pgm
	@diff -s serial.pgm thread_scheduling_mutex.pgm

serial: main.o mandel/base.o mandel/serial.o
	$(CC) $(CFLAGS) -o $@ $^

thread: main.o mandel/base.o mandel/thread.o
	$(CC) $(CFLAGS) -lpthread -o $@ $^

fork_pipe: main.o mandel/base.o mandel/fork_pipe.o
	$(CC) $(CFLAGS) -o $@ $^

thread_scheduling_mutex: main.o mandel/base.o mandel/thread_scheduling_mutex.o
	$(CC) $(CFLAGS) -lpthread -o $@ $^

clean:
	@rm -fv *.o
	@rm -fv *.pgm
	@rm -fv mandel/*.o
	@rm -fv $(TARGETS)

%.o: %.c
	$(CC) $(CFLAGS) -c -o $@ $<