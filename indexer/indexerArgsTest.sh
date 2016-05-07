#!/bin/bash
#
# indexerArgsTest.sh - test indexer command line argument validation
#
# usage: indexerArgsTest
# (no arguments)
#
# Robin Jayaswal, Kyle Dotterrer, May 2016

if [ $# != 0 ]; then
	echo usage: indexerArgsTest
	exit 1
fi
echo

# case 1: invalid number of arguments
echo "running indexer with too few arguments:"
echo ">> indexer"
./indexer 
echo "error code: $?"
echo

# case 2: invalid number of arguments
echo "running indexer with too many arguments:"
echo ">> indexer arg1 arg2 arg3"
./indexer arg1 arg2 arg3
echo "error code: $?"
echo

# case 3: invalid page directory
echo "running indexer with invalid page directory:"
echo ">> indexer ../data/badDir testIndex"
./indexer ../data/badDir ../data/testIndex
echo "error code: $?"
echo

# case 4: successful run
echo "running indexer with valid arguments:"
echo ">> indexer ../data/crawl1 testIndex"
./indexer ../data/crawl1 ../data/testIndex
echo "error code: $?"
echo

# cleanup
rm -f ../data/testIndex



