# Makefile for crawler sub-program
#
# Template courtesy of David Kotz, April 2016
# 
# Robin Jayaswal, Kyle Dotterrer, April 2016

C = ../common
L = ../lib

PROG = crawler
OBJS = crawler.o 
LIBS = -lcurl
LLIBS = $L/cs50ds.a $C/common.a

CFLAGS = -Wall -pedantic -std=c11 -ggdb -I$C -I$L $(LOG) 
CC = gcc
MAKE = make

# build the crawler
$(PROG): $(OBJS) $(LLIBS)
	$(CC) $(CFLAGS) $^ $(LIBS) -o $@

# crawler source dependencies; add others as needed
crawler.o: $C/web.h $L/hashtable/hashtable.h $L/bag/bag.h $L/memory/memory.h

# nondefault build, with logged process output
log:
	$(MAKE) LOG=-DLOG

clean:
	rm -f *~
	rm -f *.o
	rm -f $(PROG)


