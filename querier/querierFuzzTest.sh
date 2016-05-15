#!/bin/bash
#
# querierFuzzTest.sh
# script that sends a variety of queries to qeurier.
# word count ranges from 0 to 100
# words are pulled from unix dictionary

NUM_QUERIES=100

if [ $# != 2 ]; then
    echo usage: querierFuzzTest pageDirectory indexFilename
    exit 1
else
	pageDir="$1"
    index="$2"
    tmpQueries="queries.tmp"
fi

# clean up at end, and on any interruption.
trap "rm -f $tmpQueries" EXIT

echo generating $NUM_QUERIES queries

count=1
while [ $count -le $NUM_QUERIES ]
do
	let wordCount=$(($RANDOM % 25))
	echo $count
	currWC=1

	while [ $currWC -le $wordCount ]
	do
		let rand=$(($RANDOM % 10))
		if [ $rand -le 2 ]; then
			printf "%s" "and " >> "$tmpQueries"
		elif [ $rand -le 2 ]; then
			printf "%s" "or " >> "$tmpQueries"
		else
			n=$(cat /usr/share/dict/words | wc -l)
			numline=$((($RANDOM * $RANDOM) % n))
			word=$(cat -n /usr/share/dict/words | grep -w "^\s*$numline" | cut -f2)
			printf "%s" "$word " >> "$tmpQueries"
		fi
		let "currWC++"
	done
	echo >> "$tmpQueries"
	let "count++"
done

./querier $pageDir $index < "$tmpQueries"


if [ $? != 0 ]; then
	echo "Error in Fuzz Test"
else
	echo "Queries were successful"
fi