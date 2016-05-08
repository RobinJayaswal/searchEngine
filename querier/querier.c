/* ============================================================================
 *
 * querier.c
 *
 * Robin Jayaswal, Kyle Dotterrer, May 2016
 *
 =========================================================================== */

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

/************** TYPE DEFINITIONS ***************/
typedef struct countersPair {
	counters_t *intersection;
	counters_t *ctrs2;
} countersPair_t;

typedef struct result {
	int docID;
	int score;
} result_t;

/************* Function Prototypes *************/
static void printFunc(void *arg, int key, int count);
static counters_t *performQuery(char **tokens, hashtable_t *index);
static counters_t *intersectCounters(counters_t *ctrs1, counters_t *ctrs2);
static void intersectHelper(void *arg, int key, int count);
static void unionCounters(counters_t *ctrs1, counters_t *ctrs2);
static void unionHelper(void *arg, int key, int count);
static result_t **sortResults(counters_t *results, int *numResults);
static void countCounters(void *arg, int key, int count);
static void insertCounter(void *arg, int key, int count);
int resultCompare(const void *a, const void *b);
static char **tokenize(char *query);
static void freeTokensArray(char **tokens);
static bool validateQuery(char **tokens);
static int parseArguments(const int argc, char *argv[]);
static bool isCrawlerDirectory(char *dir);
static int lines_in_file(FILE *fp);
static int numDigits(int number);
static int arrayLength(char **array);
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

 		printf("query: %s\n", query);

 		char **tokens = tokenize(query);
 		if (!tokens) {
 			free(query);
 			continue;
 		}

 		if (!validateQuery(tokens)) {
 			freeTokensArray(tokens);
 			free(query);
 			continue;
 		}

 		counters_t *pageResults = performQuery(tokens, index);
 		
 		counters_iterate(pageResults, printFunc, stdout);
 		int numResults = 0;
 		result_t **sortedResults = sortResults(pageResults, &numResults);

 		for (int i = numResults-1; i >= 0; i--){
 			printf("%d\n", sortedResults[i]->docID);
 		}


 		freeTokensArray(tokens);
 		free(pageResults);
 		free(query);
 	}

 	hashtable_delete(index);

 	exit(0);
 }

 static void printFunc(void *arg, int key, int count)
 {
 	FILE *fp = arg;
 	fprintf(fp, "docID: %d count: %d\n", key, count);
 }


static counters_t *performQuery(char **tokens, hashtable_t *index) 
{
	int arrLen = arrayLength(tokens);
	int i = 0;

	counters_t *finalResults = counters_new();

	while (i < arrLen) {

		counters_t *andSequence = hashtable_find(index, tokens[i]);

		char *word;

		while ( (i < arrLen) && strcmp( ( word = tokens[i] ), "or") != 0 ) {
			if (strcmp(word, "and") == 0){
				i++;
			}
			else {
				// find docs with that word, intersect with total andSequence counters
				counters_t *currWordResults = hashtable_find(index, word);
				andSequence = intersectCounters(andSequence, currWordResults);
				i++;
			}
		}
		unionCounters(finalResults, andSequence);
		free(andSequence);
		i++;
	}

	return finalResults;
}

/*
 * intersectCounters: intersect ctrs2 with ctrs1, manipulating
 * ctrs1 in place
 */
static counters_t *intersectCounters(counters_t *ctrs1, counters_t *ctrs2)
{
	counters_t *intersection = counters_new();

	countersPair_t *pair = count_malloc_assert(sizeof(struct countersPair), MALLOC_ERROR);
	pair->intersection = intersection;
	pair->ctrs2 = ctrs2;


	counters_iterate(ctrs1, intersectHelper, pair);

	free(pair);

	return intersection;
}

/*
 * intersectHelper:
 */
static void intersectHelper(void *arg, int key, int count) 
{
	countersPair_t *pair  = arg;

	int count2 = counters_get(pair->ctrs2, key);

	if ( count2 != 0) {
		int minCount = count < count2 ? count : count2;
		counters_set(pair->intersection, key, minCount);
	}
}

/*
 * unionCounters:
 */
static void unionCounters(counters_t *ctrs1, counters_t *ctrs2)
{
	counters_iterate(ctrs2, unionHelper, ctrs1);
}

/*
 * unionHelper:
 */
static void unionHelper(void *arg, int key, int count)
{
	counters_t *ctrs1 = arg;

	int sum = count + counters_get(ctrs1, key);

	counters_set(ctrs1, key, sum);
}

static result_t **sortResults(counters_t *results, int *numResults) 
{

	counters_iterate(results, countCounters, numResults);

	result_t **sorted = count_calloc(*numResults, sizeof(result_t*));

	counters_iterate(results, insertCounter, sorted);

	qsort(sorted, *numResults, sizeof(result_t*), resultCompare);

	return sorted;
}

static void countCounters(void *arg, int key, int count)
{
	int *countersCount = ((int *)arg);

	(*countersCount)++;
}

static void insertCounter(void *arg, int key, int count)
{
	static int index = 0;

	result_t **results = arg;
	if (results == NULL || results[0] == NULL){
		index = 0;
	}
	result_t *newResult = count_malloc_assert(sizeof(struct result), MALLOC_ERROR);

	newResult->docID = key;
	newResult->score = count;

	results[index] = newResult;

	index++;
}

int resultCompare(const void *a, const void *b)
{
	const result_t *res1 = *( (result_t **) a );
	const result_t *res2 = *( (result_t **) b );

	return (res1->score - res2->score);
}

/*
 *
 */
static char **tokenize(char *query) 
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
 				for (int j = 1; j < wordCount; j++)
 					free(tokens[j]);
 				free(tokens);
 				return NULL;
 			} 
 			word[i] = tolower(word[i]);
 		}

 		tokens[wordCount] = count_malloc_assert(strlen(word) + 1, MALLOC_ERROR);
 		strcpy(tokens[wordCount], word);
 	
 		word = strtok(NULL, " ");
 	}

 	char *arrLenStr = count_malloc_assert(numDigits(wordCount) + 2, MALLOC_ERROR);
 	sprintf(arrLenStr, "%d", wordCount);

 	tokens[0] = count_malloc_assert(strlen(arrLenStr) + 1, MALLOC_ERROR);
 	
 	// store array length in first position
 	strcpy(tokens[0], arrLenStr);
 	count_free(arrLenStr);

 	// return pointer to first element of actual array
 	return ++tokens;
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




 static void freeTokensArray(char **tokens)
 {
 	
	int arrLen = arrayLength(tokens);
 	
 	for (int i = 0; i < arrLen; i++) {
 		if (tokens[i] != NULL)
 			count_free(tokens[i]);
 	}
 	// free the length of array
 	count_free(*(tokens - 1));
 	// must free from start of original allocated array
 	count_free(tokens - 1);

 	printf("Freed tokens array\n");
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
 	fclose(fp);
 	
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
static int numDigits(int number)
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
static int arrayLength(char **array)
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
