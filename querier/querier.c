/* ============================================================================
 *
 * querier.c
 *
 * Robin Jayaswal, Kyle Dotterrer, May 2016
 *
 =========================================================================== */

/* To use */
/************* *******************/

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>
#include "../common/index.h"
#include "../common/file.h"
#include "../lib/memory/memory.h"
#include "../lib/counters/counters.h"
#include "../lib/hashtable/hashtable.h"

/****************** CONSTANTS *******************/
char *MALLOC_ERROR = "Error: memory allocation error";

/************* Function Prototypes *************/
static char **tokenize(char *query);
static void freeTokensArray(char **tokens);
static bool validateQuery(char **tokens);
static int parseArguments(const int argc, char *argv[]);
static bool isCrawlerDirectory(char *dir);
static int lines_in_file(FILE *fp);
int numDigits(int number);
int arrayLength(char **array);
static void hashDeleteFunc(void *data);


 int main(const int argc, char *argv[])
 {
 	// parse arguments
 	int argStatus = parseArguments(argc, argv);
 	if (argStatus != 0)
 		exit(argStatus);

 	// argument tests passed
 	char *pageDir = argv[1];
 	char *indexFN = argv[2];

 	FILE *indexFile = fopen(indexFN, "r");
 	int numLines = lines_in_file(indexFile);
 	fclose(indexFile);

 	hashtable_t *index = hashtable_new(numLines * 1.4, hashDeleteFunc);

 	int loadStatus = indexLoad(indexFN, index);
 	if ( loadStatus != 0 ) {
 		exit(loadStatus);
 	}

 	char *query;

 	while ( (query = readline(stdin)) != NULL) {

 		char **tokens = tokenize(query);
 	
 		if (!tokens)
 			exit(11);

 		if (!validateQuery(tokens)) {
 			freeTokensArray(tokens);
 			continue;
 		}
 
 		freeTokensArray(tokens);
 	}


 	exit(0);
 }

char **tokenize(char *query) 
{
 	char *word;
 	int wordCount = 0;
 	int arrLen = 10;
 	char **tokens = count_calloc(arrLen, sizeof(char*));

 	word = strtok(query, " ");
 	
 	while( word != NULL) {

 		if (++wordCount > arrLen) {
 			arrLen = arrLen * 2;
 			tokens = realloc(tokens, arrLen);
 		}
 		for (int i = 0; i < strlen(word); i++) {
 			
 			if (isalpha(word[i]) == 0) {
 				fprintf(stderr, "Error: bad char '%c' in query\n", word[i]);
 				freeTokensArray(tokens);
 				return NULL;
 			} 
 			word[i] = tolower(word[i]);
 		}

 		tokens[wordCount] = count_malloc_assert(strlen(word) + 1, MALLOC_ERROR);
 		strcpy(tokens[wordCount], word);
 	
 		word = strtok(NULL, " ");
 	}

 	char *arrLenStr = malloc(numDigits(wordCount) + 2);
 	sprintf(arrLenStr, "%d", wordCount);

 	tokens[0] = count_malloc_assert(strlen(arrLenStr), MALLOC_ERROR);
 	// store array length in first position
 	strcpy(tokens[0], arrLenStr);

 	// return pointer to first element of actual array
 	return ++tokens;
 }

 static void freeTokensArray(char **tokens)
 {
 	int arrLen = arrayLength(tokens);
 	
 	for (int i = 0; i < arrLen; i++) {
 		if (tokens[i] != NULL)
 			count_free(tokens[i]);
 	}
 	// must free from start of original allocated array
 	count_free(tokens - 1);
 }

 static bool validateQuery(char **tokens) 
 {
 	int arrLen = arrayLength(tokens);
 	char *first = tokens[0];
 	char *last = tokens[arrLen - 1];

 	if ( (strcmp(first, "and") == 0) || (strcmp(first, "or") == 0) ) {
 		fprintf(stderr, "Error: '%s' cannot be first\n", first);
 		return false;
 	} 
 	else if ( (strcmp(last, "and") == 0) || (strcmp(last, "or") == 0) ) {
 		fprintf(stderr, "Error: '%s' cannot be last\n", last);
 		return false;
 	}

 	bool prevWasOp = false;

 	for (int i = 1; i < arrLen; i++) {
 		char *current = tokens[i];

 		if ( (strcmp(current, "and") == 0) || (strcmp(current, "or") == 0) ) {

 			if (prevWasOp) {
 				fprintf(stderr, "Error: '%s' and '%s' cannot be adjacent\n", 
 					tokens[i - 1], current);
 				return false;
 			}
 			prevWasOp = true;
 		} 
 		else {
 			prevWasOp = false;
 		}
 	}
 	return true; 
 }


/* parseArguments: check arguments passed to querier
 * returns nonzero if incorrect number of invalid pageDirectory
 * passed. zero otherwise
 */ 
 static int parseArguments(const int argc, char *argv[])
 {
 	char *programName = argv[0];
 	if (argc != 3) {
 		fprintf(stderr, "Error: Incorrect number of arguments\n");
 		fprintf(stderr, "Usage: %s pageDirectory indexFilename\n", programName);
 		return 1;
 	}

 	char *pageDir = argv[1];
 	char *indexFN = argv[2];

 	if (!isCrawlerDirectory(pageDir)){
 		fprintf(stderr, "Error: %s is not a proper crawler directory\n", pageDir);
 		fprintf(stderr, "See querier.c README for crawler directory reqs\n");
 		return 7;
 	}

 	FILE *fp;

 	if ( (fp = fopen(indexFN, "r")) == NULL) {
 		fprintf(stderr, "Error: %s is not a readable file\n", indexFN);
 		fprintf(stderr, "See querier.c README.md for index filename reqs\n");
 		return 5;
 	}
 	
 	// arguments valid to go ahead
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
 * arrayLength: return size of array
 */
int arrayLength(char **array)
{
	char *lengthStr = *(array - 1);
	int length = strtol(lengthStr, NULL, 10);
	return length;
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
