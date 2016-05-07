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


/************ FUNCTION PROTOTYPES **************/
static int parseArguments(const int argc, char *argv[]);
static void hashDeleteFunc(void *data);
static int lines_in_file(FILE *fp);


/******************* main() ********************/
int main(const int argc, char *argv[])
{
	int argStatus;
	char *oldIndexFn;
	char *newIndexFn;
	int numLines;        // number of lines in index file 
	int numSlots;        // number of slots in hashtable
	FILE *fp;

	argStatus = parseArguments(argc, argv);

	if (argStatus != 0)
		// invalid arguments
		exit(argStatus);

	oldIndexFn = argv[1];
	newIndexFn = argv[2];

	// attempt to open the file
	if ( (fp = fopen(oldIndexFn, "r")) == NULL) {
		// failure to open file
		fprintf(stderr, "Error: could not open file %s\n", oldIndexFn);
		exit(3);
	} else {
		// successful open, determine number of lines
		numLines = lines_in_file(fp);
		fclose(fp);
	}

	// calculate optimal table size: load factor range 0.7 - 0.75
	numSlots = numLines * 1.4;

	// create the inverted index data structure
	hashtable_t *index = hashtable_new(numSlots, hashDeleteFunc);
	if (index == NULL) {
		// check for allocation error
		fprintf(stderr, "Error: failed to create index structure\n");
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

	if (argc != 3){
		// invalid: must have exactly 3 arguments
		fprintf(stderr, "Error: invalid arguments\n");
		fprintf(stderr, "usage: %s oldIndexFilename newIndexFilename\n", progName);
		return 1;
	}
	return 0;
}

/**************** lines_in_file ****************/
/* Returns the number of lines in the given file,
 * i.e., the number of newlines in the file.
 * (If the file does not end with a newline, it will undercount by one.)
 * On return, the file pointer is back to beginning of file.
 * This will not work for pipes or stdin when it is attached to keyboard.
 * Courtesy of David Kotz
 */
static int lines_in_file(FILE *fp)
{
  if (fp == NULL)
    return 0;

  rewind(fp);

  int nlines = 0;
  char c = '\0';
  while ( (c = fgetc(fp)) != EOF)
    if (c == '\n')
      nlines++;

  rewind(fp);
  
  return nlines;
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

