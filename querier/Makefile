# makefile for querier subsystem
#
# Robin Jayaswal, Kyle Dotterrer, May 2016

C = ../common
L = ../lib

PROG = querier
OBJS = querier.o 
LIBS = -lcurl
LLIBS = $L/cs50ds.a $C/common.a

CFLAGS = -Wall -pedantic -std=c11 -ggdb -I$C -I$L $(LOG)
CC = gcc
MAKE = make

# build the querier
$(PROG): $(OBJS) $(LLIBS)
	$(CC) $(CFLAGS) $^ $(LIBS) -o $@

# querier source dependencies
querier.o: $C/index.h $C/file.h $L/memory/memory.h $L/hashtable/hashtable.h $L/counters/counters.h

# nondefault build, with logged process output
log:
	$(MAKE) LOG=-DLOG

clean:
	rm -f *~
	rm -f *.o
	rm -f $(PROG)

