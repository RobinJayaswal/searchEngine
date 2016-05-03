# Makefile for indexer sub-program
#
# Template courtesy of David Kotz, April 2016
# 
# Robin Jayaswal, Kyle Dotterrer, April 2016

C = ../common
L = ../lib

PROG = indexer
OBJS = indexer.o 
LIBS = -lcurl
LLIBS = $L/cs50ds.a $C/common.a

CFLAGS = -Wall -pedantic -std=c11 -ggdb -I$(LLIBS) $(LOG)
CC = gcc
MAKE = make

# build the indexer
$(PROG): $(OBJS) $(LLIBS)
	$(CC) $(CFLAGS) $^ $(LIBS) -o $@

# indexer source dependencies; add others as needed
indexer.o: 

# nondefault build, with logged process output
log:
	$(MAKE) LOG=-DLOG

clean:
	rm -f *~
	rm -f *.o
	rm -f $(PROG)

