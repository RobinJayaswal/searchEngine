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
 * output: Indextest takes an exisitng index file created by the
 *         indexer subsystem and reads it into an inverted index
 *  	   data structure. The program then writes this data structure 
 *         to a new index file, allowing for verification of index
 *         loading and saving capabilities. 
 *
 * functions:
 * 		> parseArguments validates command line arguments
 * 		> hashDeleteFunc deletes counters object from hashtable data
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
	int loadStatus;
	char *oldIndexFn;
	char *newIndexFn;

	argStatus = parseArguments(argc, argv);

	if (argStatus != 0)
		// invalid arguments
		exit(argStatus);

	oldIndexFn = argv[1];
	newIndexFn = argv[2];

	// create the inverted index data structure
	hashtable_t *index = hashtable_new(HASHTABLE_SIZE, hashDeleteFunc);

	if (index == NULL) {
		// check for allocation errors
		fprintf(stderr, "Error: index structure allocation\n");
		exit(11);
	}

	// load the index
	indexLoad(oldIndexFn, index);

	// save the index
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
	char *oldIndexFilename;
	FILE *fp;


	if (argc != 3){
		// invalid: must have exactly 3 arguments
		fprintf(stderr, "Error: invalid arguments\n");
		fprintf(stderr, "usage: %s oldIndexFilename newIndexFilename\n", progName);
		return 1;
	}

	oldIndexFilename = argv[1];

	if ( (fp = fopen(oldIndexFilename, "r")) == NULL) {
		// failed to open file
		fprintf(stderr, "Error: could not open index file\n");
		return 3;
	}
	fclose(fp);
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

