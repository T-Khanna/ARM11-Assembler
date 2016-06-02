CC      = gcc
CFLAGS  = -Wall -g -D_BSD_SOURCE -std=gnu99 -Werror -pedantic

.SUFFIXES: .c .o

.PHONY: all clean

all: assemble emulate

assemble: assemble.o, double_Linked_List.o, OPFunctions.o

emulate: emulate.o bitOper.o execute_helper.o gpio_helper.o

clean:
	rm -f $(wildcard *.o)
	rm -f assemble
	rm -f emulate
