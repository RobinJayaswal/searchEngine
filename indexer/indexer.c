/* ============================================================================
 *
 * indexer.c 
 *
 * Robin Jayaswal, Kyle Dotterrer, May 2016
 *
 * ========================================================================= */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "../lib/memory/memory.h"
#include "../lib/hashtable/hashtable.h"
#include "../lib/counters/counters.h"
#include "../common/file.h"
#include "../common/word.h"

char *MALLOC_ERR = "Error: memory allocation error";
const int HASHTABLE_SIZE = 10000;


static void indexBuild(char *pageDir, hashtable_t *index);
static void indexSave(char *indexFile, hashtable_t *index);
void indexWrite(void *arg, char *key, void *data);
void printCounterPair(void *arg, int key, int count);
static int parseArguments(const int argc, char *argv[]);
static bool isCrawlerDirectory(char *dir);
int numDigits(int number);
static void hashDeleteFunc(void *data);
// static void *checkPtr(void *ptr, char *message);



int main(const int argc, char* argv[])
{
	int argStatus;
	char *pageDir;
	char *indexFN;

	argStatus = parseArguments(argc, argv);

	if (argStatus != 0)
		exit(argStatus);

	pageDir = argv[1];
	indexFN = argv[2];

	hashtable_t *index = hashtable_new(HASHTABLE_SIZE, hashDeleteFunc);

	// build the index
	indexBuild(pageDir, index);

	// save to file indexFN
	indexSave(indexFN, index);

	
	hashtable_delete(index);

	exit(0);
}


/*
 * indexBuild: builds the index
 */
static void indexBuild(char *pageDir, hashtable_t *index)
{
	int docID = 1;
	char *fn = count_malloc_assert(strlen(pageDir)+3, MALLOC_ERR);
	FILE *fp;

	sprintf(fn, "%s/%i", pageDir, docID);

	while ( (fp = fopen(fn, "r")) != NULL) {

		char *html = file2string(fp);

		int pos = 2;
		char *word;
		while ( (pos = GetNextWord(html, pos, &word)) != -1 ) {
			char *normalized = NormalizeWord(word);
			//NormalizeWord(word);
			if (strlen(normalized) > 2){
				counters_t *wordCounters = hashtable_find(index, normalized);
				if (wordCounters){
					counters_add(wordCounters, docID);
				} else {
					counters_t *newC = counters_new();
					counters_add(newC, docID);
					hashtable_insert(index, normalized, newC);
				}
			}
			free(word);
		}
		fclose(fp);

		docID++;

		free(html);

		// construct new filename
		count_free(fn);
		fn = count_malloc_assert(strlen(pageDir)+numDigits(docID), MALLOC_ERR);
		sprintf(fn, "%s/%i", pageDir, docID);
	}
	count_free(fn);
}


static void indexSave(char *indexFile, hashtable_t *index)
{
	printf("Saving index\n");
	FILE *fp = fopen(indexFile, "w");
	hashtable_iterate(index, indexWrite, fp);
	fclose(fp);
}

void indexWrite(void *arg, char *key, void *data)
{
	// needs to take the the current word as arg. Also needs to
	// know if it is supposed to write the word on the line.
	// lastly, needs to know the file to write to. 
	// The key of the list is the word. The data is the counters struct

	FILE *fp = arg;
	char *word = key;
	counters_t *counters = data;

	if (word != NULL && counters != NULL){
		fprintf(fp, "%s", word);

		counters_iterate(counters, printCounterPair, fp);

		fprintf(fp, "\n");
	}
}

void printCounterPair(void *arg, int key, int count)
{
	if (arg != NULL) {
		FILE *fp = arg;
		int docID = key;

		fprintf(fp, " %d %d", docID, count);
	}
}

/*
 * parseArguments:
 */
static int parseArguments(const int argc, char *argv[])
{
	char *progName = argv[0];

	if (argc != 3) {
		fprintf(stderr, "Error: invalid arguments\n");
		fprintf(stderr, "usage: %s pageDirectory indexFilename\n", progName);
		return 1;
	}

	if (!isCrawlerDirectory(argv[1])) {
		fprintf(stderr, "Error: invalid page directory\n");
		return 3;
	}
	return 0;
}

/*
 * isCrawlerDirectory:
 */
static bool isCrawlerDirectory(char *dir)
{
	char *fn = malloc(strlen(dir) + 10);
	FILE *fp;

	// build filename string
	sprintf(fn, "%s/.crawler", dir);

	// create file, check for opening error
	if ( (fp = fopen(fn, "r")) == NULL) {
		free(fn);
		return false;
	} else {
		free(fn);
		fclose(fp);
		return true;
	}
}

/*
 * numDigits: recursively calculate
 * the number of digits in given integer
 */
int numDigits(int number)
{
	// integer division
	if ( (number / 10 ) == 0 ) {
		return 1;
	}
	else {
		return 1 + numDigits(number / 10);
	}
}


static void hashDeleteFunc(void *data)
{
	counters_t *counters = data;
	counters_delete(counters);
}

/*
 * checkPtr:
 * Credit to David Kotz for function inspiration
 */
// static void *checkPtr(void *ptr, char *message)
// {
// 	if (ptr == NULL) {
// 		fprintf(stderr, "Error: %s\n", message);
// 		exit(99);
// 	}
// 	return ptr;
// }
