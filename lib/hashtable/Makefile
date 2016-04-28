# makefile for hashtable module
#
# altered from original version:
#  no longer builds test program
#
# Kyle Dotterrer, April 2016

CC = gcc
MAKE = make
CFLAGS = -Wall -pedantic -std=c11 -ggdb

OBJS = hashtable.o jhash.o list.o
LIBS = 


hashtable.o: hashtable.h jhash.h list.h
list.o: list.h
jhash.o: jhash.h


clean:
	rm -f *~
	rm -f *.o
