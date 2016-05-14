/* ============================================================================
 *
 * indexer.c - build inverted index from documents in specified directory
 *
 * usage: indexer pageDirectory indexFilename
 *
 * where:
 * 		> pageDirectory is the directory where the files produced by
 * 		  crawler are located
 *      > indexFilename is the name of the file to which the index 
 *        will be written
 *
 * output: Indexer builds an inverted document index from a collection
 *         of text files located in the page directory. Once the index 
 *         is built, the result is written to the output file specified
 *         by the index filename argument. The format of the index file
 *         is: 
 *				WORD docID occurences [docID occurences]...
 *
 * functions:
 * 		> indexBuild: build inverted index from collection of files
 * 		> parseArguments: parse command line arguments
 * 		> isCrawlerDirectory: determine if page directory is valid
 * 		> numDigits: determine number of digits in integer
 * 		> hashDeleteFunc: delete counters from hastable data
 *
 * stdin: none
 * stdout: none
 * stderr: error messages
 *
 * Robin Jayaswal, Kyle Dotterrer, May 2016
 *
 * ========================================================================= */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>
#include "../lib/memory/memory.h"
#include "../lib/hashtable/hashtable.h"
#include "../lib/counters/counters.h"
#include "../common/file.h"
#include "../common/word.h"
#include "../common/index.h"

/****************** CONSTANTS *******************/
char *MALLOC_ERR = "Error: memory allocation error";    // error message
const int HASHTABLE_SIZE = 10000;                       // number of slots


/************* FUNCTION PROTOTYPES **************/
static void indexBuild(char *pageDir, hashtable_t *index);
static int parseArguments(const int argc, char *argv[]);
static bool isCrawlerDirectory(char *dir);
int numDigits(int number);
static void hashDeleteFunc(void *data);


/******************* main() ********************/
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
	
	if (index == NULL) {
		// check for allocation error
		fprintf(stderr, "Error: failed to create index structure\n");
		exit(11);
	}

	// build the index
	indexBuild(pageDir, index);

	// save to file indexFN
	indexSave(indexFN, index);

	// cleanup
	hashtable_delete(index);

	exit(0);
}


/*
 * indexBuild: builds the index; iterate over each
 * file in the page directory, read each word from file
 * and insert it into the index
 */
static void indexBuild(char *pageDir, hashtable_t *index)
{
	int docID = 1;
	char *fn = count_malloc_assert(strlen(pageDir)+3, MALLOC_ERR);
	FILE *fp;

	// construct filename for initial webpage file
	sprintf(fn, "%s/%i", pageDir, docID);

	while ( (fp = fopen(fn, "r")) != NULL) {
		// get files content into a string
		char *html = file2string(fp);
		char *htmlStart = html;
		int pos = 0;
		char *word;

		int i;
		int newlinesSeen = 0;
		for (i = 0; i < strlen(html); i++){
			if (html[0] == '\n'){
				newlinesSeen++;
				if (newlinesSeen == 2)
					break;
			}
			html++;
		}			

		// loop through words in html, normalize, update index
		while ( (pos = GetNextWord(html, pos, &word)) > 0 ) {
			char *normalized = NormalizeWord(word);
			
			if (strlen(normalized) > 2){

				// variable NULL if word not found
				counters_t *wordCounters = hashtable_find(index, normalized);

				if (wordCounters){
					// word already in index, increment counter (or create new)
					counters_add(wordCounters, docID);
				} else {
					counters_t *newC = counters_new();
					counters_add(newC, docID);
					hashtable_insert(index, normalized, newC);
				}
			}
			// free word on every iteration
			free(word);
		}
		// close file and free strings that will take new values
		fclose(fp);

		html = htmlStart;
		free(html);
		count_free(fn);

		docID++;

		// construct new filename for next iteration
		fn = count_malloc_assert(strlen(pageDir)+numDigits(docID) + 3, MALLOC_ERR);

		char *slash;
		// get last char in page dir
		char *lastChar = pageDir + strlen(pageDir) - 1;
		// determine if we should add slash to end of pageDir or nod
		slash = (*lastChar == '/') ? "" : "/";
		sprintf(fn, "%s%s%i", pageDir, slash, docID);
	}
	count_free(fn);
}

/*
 * parseArguments: validate command line arguments
 * passed to indexer, return nonzero integer if
 * arguments are invalid
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
 * isCrawlerDirectory: return true if the
 * page directory contains the .crawler
 * file created by crawling process, false
 * otherwise
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
	// integer division checks if 1 digit
	if ( (number / 10 ) == 0 ) {
		return 1;
	}
	else {	// recursively add digits of number without the first digit
		return 1 + numDigits(number / 10);
	}
}

/*
 * hashDeleteFunc: delete the data stored
 * in the hashtable; data is counters structure,
 * so call counters_delete() on each data
 */
static void hashDeleteFunc(void *data)
{
	counters_t *counters = data;
	counters_delete(counters);
}

