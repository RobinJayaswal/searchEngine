# makefile for common.a
#  build the common library
#
# Robin Jayaswal, Kyle Dotterrer, May 2016

OBJS = file.o web.o word.o
LIB = common.a

CFLAGS = -Wall -pedantic -std=c11 -ggdb
CC = gcc
MAKE = make

# build the library
$(LIB): $(OBJS)
	ar cr $@ $^


# object file dependencies
file.o: file.c file.h

web.o: web.c web.h

word.o: word.c word.h


# clean library and subdirectories
clean:
	rm -f *~
	rm -f *.o
	rm -f $(LIB)
	