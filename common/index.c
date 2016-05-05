/* ============================================================================
 *
 * index.c
 *
 * Robin Jayaswal, Kyle Dotterrer, May 2016
 *
 * ========================================================================= */

#include <stdlib.h>
#include <stdio.h>
#include "../lib/counters/counters.h"
#include "../lib/hashtable/hashtable.h"

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
 * indexWrite:
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
 * printCounterPair: 
 */
static void printCounterPair(void *arg, int key, int count)
{
	if (arg != NULL) {
		FILE *fp = arg;
		int docID = key;

		fprintf(fp, " %d %d", docID, count);
	}
}