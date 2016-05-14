#!/bin/bash
#
# queryTest.sh - test querier's handling of problematic queries
#
# Robin Jayaswal, Kyle Dotterrer, May 2016

if [ $# != 0 ]; then
	echo "error: invalid arguments"
	echo "usage queryTest (no arguments)"
	exit 1
fi

# call querier with file-based input
./querier ../data/crawl1 ../data/index1 < queryTestInput.txt

echo
echo "error status at exit is $?"

exit 0