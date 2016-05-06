/* ============================================================================
 *
 * indextest.c - load index file into inverted index data structure
 *
 * usage: indextest oldIndexFilename newIndexFilename
 *
 * where: 
 * 		> oldIndexFilename is the name of an existing index file
 *      > newIndexFilename is the name of the file to which the 
 *        new index will be written
 *
 * output:
 *
 * functions:
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
#include "../common/index.h"
#include "../lib/counters/counters.h"
#include "../lib/hashtable/hashtable.h"

/****************** CONSTANTS *******************/
char *MALLOC_ERR = "Error: memory allocation error";    // error message
const int HASHTABLE_SIZE = 10000;                       // number of slots


/************ FUNCTION PROTOTYPES **************/
int parseArguments(const int argc, char *argv[]);
static void hashDeleteFunc(void *data);


/******************* main() ********************/
int main(const int argc, char *argv[])
{
	int argStatus;
	char *oldIndexFn;
	char *newIndexFn;

	argStatus = parseArguments(argc, argv);

	if (argStatus != 0)
		exit(argStatus);

	oldIndexFn = argv[1];
	newIndexFn = argv[2];

	hashtable_t *index = hashtable_new(HASHTABLE_SIZE, hashDeleteFunc);

	// load the index
	indexLoad(oldIndexFn, index);

	// write the index
	indexSave(newIndexFn, index);

	// cleanup
	hashtable_delete(index);

	exit(0);
}

/*
 * parseArguments: validate command line arguments
 * passed to indextester; return nonzero integer if
 * arguments are invalid, 0 otherwise
 */
int parseArguments(const int argc, char *argv[])
{
	char *progName = argv[0];

	if (argc != 3){
		fprintf(stderr, "Error: invalid arguments\n");
		fprintf(stderr, "usage: %s oldIndexFilename newIndexFilename\n", progName);
		return 1;
	}

	return 0;
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

