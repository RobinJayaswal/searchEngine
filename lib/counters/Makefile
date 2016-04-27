# makefile for counterstest
#  used explicitly for testing 'counters' module
#
# Kyle Dotterrer, April 2016

CC = gcc
CFLAGS = -Wall -pedantic -std=c11 -ggdb
PROG = counterstest
OBJS = counters.o counterstest.o
LIBS = 

#$(PROG): $(OBJS)
#	$(CC) $(CFLAGS) $^ $(LIBS) -o $@


counters.o: counters.h
#counterstest.o: counters.h


clean:
	rm -f *~
	rm -f *.o
	rm -f counterstest