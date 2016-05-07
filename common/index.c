/* ============================================================================
 *
 * index.c - implementations for index module
 *  provides functions to save inverted index to file,
 *  and load the data from file into inverted index
 *
 * Robin Jayaswal, Kyle Dotterrer, May 2016
 *
 * ========================================================================= */

#include <stdlib.h>
#include <stdio.h>
#include "../lib/counters/counters.h"
#include "../lib/hashtable/hashtable.h"
#include "file.h"

static void indexWrite(void *arg, char *key, void *data);
static void printCounterPair(void *arg, int key, int count);

/****************** indexSave *******************/
void indexSave(char *indexFile, hashtable_t *index)
{
	FILE *fp = fopen(indexFile, "w");

	// iterate over each word in index, writing to file
	hashtable_iterate(index, indexWrite, fp);
	fclose(fp);
}

/*
 * indexWrite: iterator function for hashtable containing the index
 * arg: file to print index to
 * key: word in indexer
 * data: counters struct for that word
 * Makes call to counters_iterate to print counter key/val pairs to file
 */
static void indexWrite(void *arg, char *key, void *data)
{
	FILE *fp = arg;
	char *word = key;
	counters_t *counters = data;

	if (word != NULL && counters != NULL){
		fprintf(fp, "%s", word);

		// iterate over each docID, count combo 
		counters_iterate(counters, printCounterPair, fp);

		fprintf(fp, "\n");
	}
}

/*
 * printCounterPair: iterator function for counters, 
 * prints key/count pairs to file
 * arg: file to output to
 * key and count are just the key and count of a counter
 */
static void printCounterPair(void *arg, int key, int count)
{
	if (arg != NULL) {
		FILE *fp = arg;
		int docID = key;

		fprintf(fp, " %d %d", docID, count);
	}
}

/****************** indexLoad *******************/
int indexLoad(char *indexFile, hashtable_t *index)
{
	FILE *fp;          
	char word[200];   // character buffer 
	int docID;        
	int count;

	// open file; check for errors
	if ( (fp = fopen(indexFile, "r")) == NULL) {
		fprintf(stderr, "Error: unable to read from file %s\n", indexFile);
		return 3;
	}

	// pull off the leading word
	while (fscanf(fp, "%s ", word) == 1) {

		// create new counters and insert into table
		counters_t *counters = counters_new();
		hashtable_insert(index, word, counters);

		// loop over all docID, count pairs
		while (fscanf(fp, " %d %d", &docID, &count) == 2) {

			// create new counter and set 
			counters_set(counters, docID, count);
		}
	}
	fclose(fp);
	return 0;
}





