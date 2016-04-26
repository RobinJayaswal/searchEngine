# makefile for bagtest
#  used explicitly for testing bag module
#
# Kyle Dotterrer, April 2016

CC = gcc
CFLAGS = -Wall -pedantic -std=c11 -ggdb
PROG = bagtest
OBJS = bag.o bagtest.o
LIBS = 

$(PROG): $(OBJS)
	$(CC) $(CFLAGS) $^ $(LIBS) -o $@


bag.o: bag.h
bagtest.o: bag.h


clean:
	rm -f *~
	rm -f *.o
	rm -f bagtest
