#!/bin/bash
#
# tse.sh
#
# Robin Jayaswal, Kyle Dotterrer, May 2016

if [ $# != 2 ]; then
	echo "error: invalid arguments"
	echo "usage tse seedURL maxDepth"
	exit 1
else
	seedUrl="$1"
	maxDepth="$2"
fi

# build subsystems and libraries
make > /dev/null 2>&1

mkdir -p ./data/tmpPageDir
pageDir=./data/tmpPageDir
indexFN=./data/tmpIndex

# crawl 
./crawler/crawler "$seedURL" "$pageDir" "$maxDepth" || exit 2
echo "finished crawl"

# index
./indexer/indexer "$pageDir" "$indexFN" || exit 3
echo "finished index"

# query
./querier/querier "$pageDir" "$indexFN" || exit 4

rm -f "$pageDir"/*
rm -f "$pageDir"/.crawler
rmdir "$pageDir"
rm -f "$indexFN"

make clean > /dev/null 2>&1

exit 0



