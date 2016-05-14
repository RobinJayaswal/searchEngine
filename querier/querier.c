/* ============================================================================
 *
 * querier.c - print documents matching user input query to stdout
 *
 * usage: querier pageDirectory indexFilename
 *
 * where: 
 * 		> pageDirectory is the directory containing text files creating 
 *        by crawler; directory only conisdered valid when special file
 *        .crawler is within the pageDirectory
 * 		> indexFilename is the name of the index file created by indexer
 *
 * output: querier searches the given index file for documents that satisfy
 *         a user input query and prints these results to standard output
 *         for the user to see. querier supports 'and' and 'or' operators
 *         within the query; documents must have all query terms within an
 *         andsequence to satisfy, and at least one of the terms in an 
 *         orsequence to satisfy. 'and' takes precedence over 'or'. 
 *
 * functions: functions in this file are divided into the categories:
 * 
 * 	Primary Functionality
 *  	> tokenize: parse the query string into individual tokens
 *  	> printQuery: print cleaned query for user to see
 *  	> validateQuery: check if query has invlaid constructions
 *  	> performQuery: search the index for satisfying documents
 *  	> sortResults: sort the results returned by performing query
 *  	> printResults: print the query results in human-readable format
 *  Counters Helpers
 *  	> intersectCounters: perform intersection on 2 counters structs
 *  	> intersectHelper: iterator function for intersection
 *  	> unionCounters: perform union on 2 counters structs
 *  	> unionHelper: iterator function for union
 *  	> countCounters: count the number of counters in counters struct
 *  	> insertCounter: insert counter data into specified array as struct
 *  Comparison Functions
 *  	> resultCompare: comparison function for sorting results array
 *  Memory Deallocation
 *  	> freeTokensArray: deallocate array of query tokens
 *  	> freeResults: deallocate array of results structs
 *  Auxillary Functions
 *  	> parseArguments: ensure command line args are valid 
 *  	> isCrawlerDirectory: check for presence of .crawler in pageDir
 *  	> linesInFile: count number of newlines in given file
 *  	> numDigits: return number of digits in given integer
 *  	> arrayLength: retrieve length or array stored at position 0
 *  	> hashDeleteFunc: delete data from hashtable element
 *  	> checkPtr: throw error and exit if bad pointer given
 *
 * stdin: user input search queries
 * stdout: query results
 * stderr: error messages
 *
 * Robin Jayaswal, Kyle Dotterrer, May 2016
 *
 =========================================================================== */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>
#include <signal.h>
#include "index.h"
#include "file.h"
#include "memory/memory.h"
#include "counters/counters.h"
#include "hashtable/hashtable.h"

/* ===================== TYPE DEFINITIONS ===================== */

// used to insesect two counters
typedef struct countersPair {
	counters_t *intersection;
	counters_t *ctrs2;
} countersPair_t;

// docID-score pairs for compiling results
typedef struct result {
	int docID;
	int score;
} result_t;

/* ==================== FUNCTION PROTOTYPES ==================== */

/************ Primary Functionality *************/
static char **tokenize(char *query);
static bool printQuery(char **tokens);
static bool validateQuery(char **tokens);
static counters_t *performQuery(char **tokens, hashtable_t *index);
static result_t **sortResults(counters_t *results, int *numResults);
static void printResults(result_t **results, int numResults, char *pageDir);

/************** Counters Helpers ***************/
static counters_t *intersectCounters(counters_t *ctrs1, counters_t *ctrs2);
static void intersectHelper(void *arg, int key, int count);
static void unionCounters(counters_t *ctrs1, counters_t *ctrs2);
static void unionHelper(void *arg, int key, int count);
static void countCounters(void *arg, int key, int count);
static void insertCounter(void *arg, int key, int count);

/************ Comparison Functions *************/
static int resultCompare(const void *a, const void *b);

/************* Memory Deallocation **************/
static void freeTokensArray(char **tokens);
static void freeResults(result_t **results, int len);

/************* Auxillary Functions **************/
static int parseArguments(const int argc, char *argv[]);
static bool isCrawlerDirectory(char *dir);
static int linesInFile(char *fn);
static int numDigits(int number);
static int arrayLength(char **array);
static void hashDeleteFunc(void *data);
static void *checkPtr(void *ptr, char *loc);


/* ============================ Main ============================= */


int main(const int argc, char *argv[])
{
 	// parse arguments
	int argStatus = parseArguments(argc, argv);
 	if (argStatus != 0)
 		exit(argStatus);

 	// argument tests passed
 	char *pageDir = argv[1];
 	char *indexFN = argv[2];

 	int numLines = linesInFile(indexFN);

 	// initialize the index
 	hashtable_t *index = hashtable_new(numLines * 1.4, hashDeleteFunc);

 	// load the index from file, check for errors
 	int loadStatus = indexLoad(indexFN, index);
 	if ( loadStatus != 0 ) {
 		exit(loadStatus);
 	}

 	char *query;

 	// readline returns NULL when EOF reached
 	while ( (query = readline(stdin)) != NULL) {

 		char **tokens = tokenize(query);
 		// tokenize returns null if invalid chars in query
 		if (!tokens) {
 			free(query);
 			continue;
 		}

 		// printQuery returns false if empty query given
 		if (!printQuery(tokens)) {
 			freeTokensArray(tokens);
 			free(query);
 			continue;
 		}

 		// validate query, checking for adjacent or beginning or ending 'and' and 'ors'
 		if (!validateQuery(tokens)) {
 			freeTokensArray(tokens);
 			free(query);
 			continue;
 		}

 		counters_t *pageResults = performQuery(tokens, index);
 		
 		// pass an integer pointer into sortResults to count elements in pageResults
 		int numResults = 0;
 		result_t **sortedResults = sortResults(pageResults, &numResults);

 		printResults(sortedResults, numResults, pageDir);

 		// clean up before next loop
 		freeTokensArray(tokens);
 		counters_delete(pageResults);
 		freeResults(sortedResults, numResults);
 		free(query);
 	}

	#ifdef LOG
 	count_report(stdout, "Memory Allocation Report");
 	#endif

 	// final cleanup and exit
 	hashtable_delete(index);

 	exit(0);
}


/* ==================== Primary Functionality ===================== */


/* tokenize: takes the query string, converts all to lower case and creates
 * array of words in query. Checks for nonalpha characters, which are invalid
 * Returns: Array of words in query, with pointer to size of array immediately
 * preceding array in memory. NULL if nonalpha char appears
 */
static char **tokenize(char *query) 
{
 	char *word;
 	int wordCount = 0;
 	// init tokens array with length likely big enough, will realloc if needed
 	int arrLen = 10;	
 	char **tokens = count_calloc(arrLen, sizeof(char*));

 	word = strtok(query, " ");
 	
 	// store words in tokens array from index 1 onwards. Index 0 will hold length
 	while( word != NULL) {

 		// increment word count because we have seen another word
 		if (++wordCount >= arrLen) {
 			// reallocate tokens array with more space if we reach current arrLen
 			arrLen = arrLen * 2;
 			tokens = realloc(tokens, arrLen * sizeof(char*));
 		}

 		for (int i = 0; i < strlen(word); i++) {
 			
 			if (isalpha(word[i]) == 0) {
 				fprintf(stderr, "Error: bad char '%c' in query\n", word[i]);
 				for (int j = 1; j < wordCount; j++)
 					free(tokens[j]);
 				free(tokens);
 				return NULL;
 			} else {
 			// convert to lower 
 			word[i] = tolower(word[i]);
 			}
 		}

 		// store the word in slot wordCount
 		tokens[wordCount] = count_malloc(strlen(word) + 1);
 		strcpy(tokens[wordCount], word);
 	
 		word = strtok(NULL, " ");
 	}

 	// create string holding the array length
 	char *arrLenStr = count_malloc(numDigits(wordCount) + 2);
 	sprintf(arrLenStr, "%d", wordCount);
	
	// store array length in first position
 	tokens[0] = count_malloc(strlen(arrLenStr) + 1);	
 	strcpy(tokens[0], arrLenStr);

 	count_free(arrLenStr);

 	// return pointer to first element of actual array
 	return ++tokens;
}

/* printQuery: outputs the lowercased normalized query to stdout
 * Returns: false if tokens array is empty, true otherwise
 */
static bool printQuery(char **tokens)
{
	int arrLen = arrayLength(tokens);

	printf("query: ");
	if (arrLen == 0) {
		printf("(empty)\nNo documents match.\n");
		printf("----------------------------------------\n");
		return false;
	}

	for (int i = 0; i < arrLen; i++) {
		printf("%s ", tokens[i]);
	}
	printf("\n");
	return true;
}

/* validateQuery: check for validity of query. The words 'and' and 'or' may not 
 * be adjacent to themselves or each other, and may not come at start or end
 * Returns: bool indicating validity of query
 */
static bool validateQuery(char **tokens) 
{
 	int arrLen = arrayLength(tokens);

 	// check first and last
 	char *first = tokens[0];
 	char *last = tokens[arrLen - 1];

 	if ( (strcmp(first, "and") == 0) || (strcmp(first, "or") == 0) ) {
 		fprintf(stderr, "Error: '%s' cannot be first\n", first);
 		printf("----------------------------------------\n");
 		return false;
 	} 
 	else if ( (strcmp(last, "and") == 0) || (strcmp(last, "or") == 0) ) {
 		fprintf(stderr, "Error: '%s' cannot be last\n", last);
 		printf("----------------------------------------\n");
 		return false;
 	}

 	// bool indicating whether previous word was operator
 	bool prevWasOp = false;

 	for (int i = 1; i < arrLen; i++) {
 		char *current = tokens[i];

 		if ( (strcmp(current, "and") == 0) || (strcmp(current, "or") == 0) ) {

 			if (prevWasOp) {	// two operators in a row
 				fprintf(stderr, "Error: '%s' and '%s' cannot be adjacent\n", 
 					tokens[i - 1], current);
 				printf("----------------------------------------\n");
 				return false;
 			}

 			prevWasOp = true;
 		} 
 		else {
 			prevWasOp = false;
 		}
 	}
 	// reached end of query without hitting invalid operator format
 	return true;
 }

/* performQuery: takes array of strings and an index. Loops through
 * query made up of the tokens array, intersecting results in and sequences
 * and unioning both sides of 'ors'. 
 * Returns: Counters object with (docID, score) pair for results matching query
 */
static counters_t *performQuery(char **tokens, hashtable_t *index) 
{
	int arrLen = arrayLength(tokens);

	// counters struct to be unioned with every and sequence and returned
	counters_t *finalResults = counters_new();

	int i = 0;
	while (i < arrLen) {

		// start off andSequence counters with all (docID, counter) pairs
		// matching the first word of and sequence
		counters_t *andSequence = hashtable_find(index, tokens[i]);

		// loop through and sequence intersecting andSequence with results
		// of each individual word
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
		// union the andSequence with the final results before finding next andSequence
		unionCounters(finalResults, andSequence);
		counters_delete(andSequence);
		i++;
	}

	return finalResults;
}

/* sortResults: takes a counters struct, and an integer pointer.
 * Creates an array of (docID, score) pairs. Sorts this array by score.
 * Inserts length of counters object, and thereby length of array into int ptr
 * Returns sorted array of (docID, score) pairs
 */
static result_t **sortResults(counters_t *results, int *numResults) 
{
	// put length of counters list as value of numResults
	counters_iterate(results, countCounters, numResults);

	result_t **sorted = count_calloc(*numResults, sizeof(result_t*));

	// insert every (docID, score) pair into sorted array
	counters_iterate(results, insertCounter, sorted);

	qsort(sorted, *numResults, sizeof(result_t*), resultCompare);

	return sorted;
}

/* printResults: prints out the query results.
 * Finds the url in the file corresponding to each result's docID.
 * Returns: void
 */
static void printResults(result_t **results, int numResults, char *pageDir)
{
	if (numResults == 0) {
 		printf("No documents match.\n");
 	}
 	else {
 		printf("Matches %d documents (ranked):\n", numResults);

 		// find necessary column width for score
 		int maxScore = results[numResults - 1]->score;
 		int scoreDigits = numDigits(maxScore);

 		// find necessary column width for docID
 		int maxID = 0;
 		for (int i = numResults - 1; i >= 0; i--) {
 			if (results[i]->docID > maxID)
 				maxID = results[i]->docID;
 		}
 		int idDigits = numDigits(maxID);

 		// print all of the results to stdout
 		for (int i = numResults - 1; i >= 0; i--) {
 			// get filename and open file corresponding to result's docID
 			char *fn = count_malloc(strlen(pageDir) + numDigits(results[i]->docID) + 2);
 			sprintf(fn, "%s/%d", pageDir, results[i]->docID);

			FILE *fp = checkPtr(fopen(fn, "r"), "Error: printResults. "
												"pageDir and indexFN not corresponding");

			// get URL of result
			char *url = checkPtr(readline(fp), "Error: printResults. "
											   "page directory file empty");
 			
 			printf("score: %*d ", scoreDigits, results[i]->score);
 			printf("doc %*d: %s\n", idDigits, results[i]->docID, url); 

 			count_free(fn);
 			fclose(fp);
 			free(url);
 		}
 	}
 	printf("----------------------------------------\n");
}


/* ====================== Counters Helpers ======================= */


/*
 * intersectCounters: intersect ctrs2 with ctrs1, manipulating
 * ctrs1 in place
 */
static counters_t *intersectCounters(counters_t *ctrs1, counters_t *ctrs2)
{
	counters_t *intersection = counters_new();

	countersPair_t *pair = count_malloc(sizeof(struct countersPair));
	
	pair->intersection = intersection;
	pair->ctrs2 = ctrs2;

	counters_iterate(ctrs1, intersectHelper, pair);

	count_free(pair);
	if (ctrs1 != ctrs2)
		counters_delete(ctrs1);
	return intersection;
}

/*
 * intersectHelper: Counters iterator function, 
 * helper function for intersectCounters. Sets count
 * of result in intersection to min of results in either counter
 * if both found
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
 * unionCounters: union contents of two counters, summing scores for
 * results found in both
 */
static void unionCounters(counters_t *ctrs1, counters_t *ctrs2)
{
	counters_iterate(ctrs2, unionHelper, ctrs1);
}

/*
 * unionHelper: sums the counts of overlapping results
 */
static void unionHelper(void *arg, int key, int count)
{
	counters_t *ctrs1 = arg;

	int sum = count + counters_get(ctrs1, key);

	counters_set(ctrs1, key, sum);
}

/* countCounters: counters iterate func. increments count of arg for each
 * counter in counters
 */  
static void countCounters(void *arg, int key, int count)
{
	int *countersCount = ((int *)arg);

	(*countersCount)++;
}

/* insertCounter: counters iterate func. Inserts result struct
 * holding (docID, count) pair for each counter into array held by
 * results
 */
static void insertCounter(void *arg, int key, int count)
{
	static int index = 0;

	result_t **results = arg;
	if (results == NULL || results[0] == NULL){
		index = 0;		// reset index if we are starting a new results array
	}

	result_t *newResult = count_malloc(sizeof(struct result));

	newResult->docID = key;
	newResult->score = count;

	results[index] = newResult;

	index++;
}


/* ==================== Comparison Functions ===================== */


/* resultCompare: helper function for qsort. Takes two result structs
 * and returns positive if score of first result higher than second,
 * 0 if equal, negative otherwise
 */
static int resultCompare(const void *a, const void *b)
{
	const result_t *res1 = *( (result_t **) a );
	const result_t *res2 = *( (result_t **) b );

	return (res1->score - res2->score);
}


/* ==================== Memory Deallocation ====================== */


/* freeResults: loop over results and free each result, then free results
 * Return: void
 */
static void freeResults(result_t **results, int len)
{
	for (int i = 0; i < len; i++) {
		if (results[i])
			count_free(results[i]);
	}
	count_free(results);
}

/* freeTokensArray: free all the tokens in tokens array,
 * then decrement pointer and free since it was initially
 * malloced one bigger to hold size of array
 */
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
}


/* ===================== Auxillary Functions ====================== */


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

/* Returns the number of lines in the given file,
 * i.e., the number of newlines in the file.
 * (If the file does not end with a newline, it will undercount by one.)
 * Handles both file opening and closing
 * Function Inspiration Courtesy of David Kotz
 */
static int linesInFile(char *fn)
{
	FILE *fp;

  	if ( (fp = fopen(fn, "r")) == NULL) {
  		fprintf(stderr, "Error: %s is not a readable file\n", fn);
  		return NULL;
  	}

  	rewind(fp);

  	int nlines = 0;
  	char c = '\0';
  	while ( (c = fgetc(fp)) != EOF) {
    	if (c == '\n')
      		nlines++;
  	}

  	fclose(fp);
  
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

/*
 * checkPtr: checks if pointer null. 
 * Exits if null, returns pointer otherwise
 */
static void * checkPtr(void *ptr, char *loc) 
{
	if (ptr == NULL) {
		fprintf(stderr, "Error: unexpected NULL at %s\n", loc);
		exit(99);
	}
	else
		return ptr;
}



