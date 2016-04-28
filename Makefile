# Makefile for 'crawler' module
#
# Template courtesy of David Kotz, April 2016
# 
# Robin Jayaswal, Kyle Dotterrer, April 2016

PROG = crawler
OBJS = crawler.o web.o
LIBS = -lcurl
LLIBS = lib/cs50ds.a

CFLAGS = -Wall -pedantic -std=c11 -ggdb
CC = gcc
MAKE = make

# build the crawler
$(PROG): $(OBJS) $(LLIBS)
	$(CC) $(CFLAGS) $^ $(LIBS) -o $@

# crawler source dependencies; add others as needed
crawler.o: web.h
web.o: web.h

# build the library
lib/cs50ds.a: 
	cd lib; $(MAKE)

clean:
	rm -f *~
	rm -f *.o
	rm -f $(PROG)
	cd lib; $(MAKE) clean
	cd data; rm -f *
