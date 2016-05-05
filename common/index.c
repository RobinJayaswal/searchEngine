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
	char *line;
	
	char *word;
	int docID;
	int count;

	if (index == NULL) {
		fprintf(stderr, "Error: cannot load into invalid structure\n");
		return 11;
	}

	if ( (fp = fopen(indexFile, "r")) == NULL) {
		// failed to open file
		fprintf(stderr, "Error: could not open index file\n");
		return 13;
	}

	// loop over every line in index file
	while ( (line = readline(fp)) != EOF) {

		// pull off the word
		sscanf(line, "%s", word);

		// loop over (docID, count) pairs
		while (sscanf(line, "%d %d", &docID, &count) == 2) {
			;
		}

		// free the line
		free(line);
	}
}


