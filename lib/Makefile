# Makefile for cs50ds library
#   Builds and compiles all the data structures into a library.
#
# Template courtesy of David Kotz, April 2016
#
# Robin Jayaswal, Kyle Dotterrer, April 2016

OBJS = memory/memory.o list/list.o hashtable/hashtable.o 
OBJS = hashtable/jhash.o hashtable/list.o bag/bag.o counters/counters.o
LIB = cs50ds.a

CFLAGS = -Wall -pedantic -std=c11 -ggdb
CC = gcc
MAKE = make

# build the library
$(LIB): $(OBJS)
	ar cr $@ $^

# rules for building each object file
memory/memory.o: memory/memory.c memory/memory.h
	cd memory; $(MAKE) memory.o

bag/bag.o: bag/bag.c bag/bag.h
	cd bag; $(MAKE) bag.o

counters/counters.o: counters/counters.c counters/counters.h
	cd counters; $(MAKE) counters.o

list/list.o: list/list.c list/list.h
	cd list; $(MAKE) list.o

hashtable/hashtable.o: hashtable/hashtable.c hashtable/hashtable.h
	cd hashtable; $(MAKE) hashtable.o

# clean library and subdirectories
clean:
	rm -f *~
	rm -f *.o
	rm -f $(LIB)
	cd memory; $(MAKE) $@
	cd bag; $(MAKE) $@
	cd counters; $(MAKE) $@
	cd list; $(MAKE) $@
	cd hashtable; $(MAKE) $@
