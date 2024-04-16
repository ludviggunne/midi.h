
CC=gcc
CFLAGS=-Wall -Wextra -Wpedantic -Werror

.PHONY: clean

example.mid: example
	./example > example.mid

example: example.c midi.h
	$(CC) $(CFLAGS) -o $@ $<

clean:
	rm -f example *.o
