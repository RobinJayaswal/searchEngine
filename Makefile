# makefile for tiny search engine
#  recursively build all subdirectories
#
# Robin Jayaswal, Kyle Dotterrer, May 2016 


MAKE = make

all:
	# build the libraries
	$(MAKE) -C lib
	$(MAKE) -C common

	# build programs
	$(MAKE) -C crawler
	$(MAKE) -C indexer
	$(MAKE) -C querier

clean:
	cd common; $(MAKE) clean
	cd lib; $(MAKE) clean
	cd crawler; $(MAKE) clean
	cd indexer; $(MAKE) clean
	cd querier; $(MAKE) clean