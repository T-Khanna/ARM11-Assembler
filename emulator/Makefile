CC      = gcc
CFLAGS  = -Wall -g -D_BSD_Source -std=gnu99 -Werror -pedantic

.SUFFIXES: .c .o

.PHONY: clean

emulate: ../bitOpers/bitOper.o emulate.o execute_helper.o gpio_helper.o

clean:
	rm -f $(wildcard *.o)
	rm -f emulate
