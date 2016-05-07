# makefile for tiny search engine
#
# recursively build programs in subdirectories and libraries
#
# Robin Jayaswal, Kyle Dotterrer, May 2016 


MAKE = make

# default target, recursively build
all:
	# build the libraries
	$(MAKE) -C lib
	$(MAKE) -C common

	# build programs
	$(MAKE) -C crawler
	$(MAKE) -C indexer
	$(MAKE) -C querier

# nondefault, logged output in crawler
log: 
	# build the libraries
	$(MAKE) -C lib
	$(MAKE) -C common

	# build programs
	$(MAKE) log -C crawler
	$(MAKE) -C indexer
	$(MAKE) -C querier

# recursively clean each subdirectory
clean:
	cd common; $(MAKE) clean
	cd lib; $(MAKE) clean
	cd crawler; $(MAKE) clean
	cd indexer; $(MAKE) clean
	cd querier; $(MAKE) clean