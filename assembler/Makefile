CC			= gcc
CFLAGS	= -Wall -g -D_BSD_SOURCE -std=gnu99 -Werror -pedantic

.SUFFIXES:	.c .o

.PHONY: clean

assemble: ../bitOpers/bitOper.o assemble.o labeliser.o tokeniser.o assemble_helper.o OPFunctions.o 
clean:
	rm -f $(wildcard *.o)
	rm -f assemble
