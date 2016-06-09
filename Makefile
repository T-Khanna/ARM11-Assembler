CC      = gcc
CFLAGS  = -Wall -g -D_BSD_SOURCE -std=gnu99 -Werror -pedantic

.SUFFIXES: .c .o

.PHONY: all clean

all: assemble emulate

assemble:
	make assemble -C assembler/
	cp assembler/assemble .

emulate: 
	make emulate -C emulator/
	cp emulator/emulate .

clean:
	rm -f $(wildcard *.o)
	rm -f assemble emulate
	make clean -C assembler/
	make clean -C emulator/
