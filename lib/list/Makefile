# makefile for listtest
#  used explicitly for testing list module
#
# Kyle Dotterrer, April 2016

CC = gcc
CFLAGS = -Wall -pedantic -std=c11 -ggdb
PROG = listtest
OBJS = list.o listtest.o
LIBS = 

$(PROG): $(OBJS)
	$(CC) $(CFLAGS) $^ $(LIBS) -o $@


list.o: list.h
listtest.o: list.h


clean:
	rm -f *~
	rm -f *.o
	rm -f listtest