# Makefile for indexer subsystem and indextest program
#
# Template courtesy of David Kotz, April 2016
# 
# Robin Jayaswal, Kyle Dotterrer, April 2016

C = ../common
L = ../lib

PROG = indexer 
OBJS = indexer.o

PROG2 = indextest
OBJS2 = indextest.o

LIBS = 
LLIBS = $L/cs50ds.a $C/common.a

CFLAGS = -Wall -pedantic -std=c11 -ggdb -I$C -I$L $(LOG)
CC = gcc
MAKE = make

all: $(PROG) $(PROG2)
	
# build the indexer
$(PROG): $(OBJS) $(LLIBS)
	$(CC) $(CFLAGS) $^ $(LIBS) -o $@

$(PROG2): $(OBJS2) $(LLIBS)
	$(CC) $(CFLAGS) $^ $(LIBS) -o $@

# indexer source dependencies
indexer.o: $L/memory/memory.h $L/hashtable/hashtable.h $L/counters/counters.h $C/index.h $C/word.h $C/file.h

# indextest source dependencies
indextest.o: $C/index.h $L/hashtable/hashtable.h $L/counters/counters.h

# nondefault build, with logged process output
log:
	$(MAKE) LOG=-DLOG

clean:
	rm -f *~
	rm -f *.o
	rm -f $(PROG)
	rm -f $(PROG2)


