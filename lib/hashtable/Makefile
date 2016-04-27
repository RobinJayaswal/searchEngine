# makefile for hashtabletest
#  used explicitly for testing 'hashtable' module
#
# Kyle Dotterrer, April 2016

CC = gcc
CFLAGS = -Wall -pedantic -std=c11 -ggdb
PROG = hashtabletest
OBJS = hashtable.o hashtabletest.o jhash.o list.o
LIBS = 

#$(PROG): $(OBJS)
#	$(CC) $(CFLAGS) $^ $(LIBS) -o $@


hashtable.o: hashtable.h jhash.h
#hashtabletest.o: hashtable.h
list.o: list.h
jhash.o: jhash.h


clean:
	rm -f *~
	rm -f *.o
	rm -f hashtabletest