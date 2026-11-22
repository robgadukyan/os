CC = gcc
CFLAGS = -std=c11 -Wall -Wextra -Wpedantic -O2
PROGRAMS = basic_threads array_sum print_numbers parallel_squares

.PHONY: all clean

all: $(PROGRAMS)

$(PROGRAMS): %: %.c threads.h
	$(CC) $(CPPFLAGS) $(CFLAGS) $(LDFLAGS) -pthread $< -o $@ $(LDLIBS)

clean:
	$(RM) $(PROGRAMS)
