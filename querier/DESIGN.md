## **DESIGN for querier subsytem**
## Robin Jayaswal, Kyle Dotterrer, May 2016

### **Requirements Spec**

The TSE querier is a standalone program that reads the index file produced by the TSE indexer, and answers search queries submitted via stdin.

The querier **shall**:

1. execute from a command line with usage syntax 
	* ```./querier pageDirectory indexFilename```
	* where ```pageDirectory``` is a directory produced by the crawler, and
	* where ```indexFilename``` is an index file produced by the indexer.

2. load the index from indexFilename into an internal data structure.

3. read search queries from stdin, one per line, until EOF.
	* clean and parse each query according to the syntax described below.
	* print the ‘clean’ query for user to see.
	* use the index to identify the set of documents that satisfy the query, as described below.
	* if no documents satisfy the query, print No documents match.
	* otherwise, rank the resulting set of documents according to its score, as described below, and print the set of documents in decreasing rank order; for each, list the score, document ID and URL. (Obtain the URL by reading the first line of the relevant document file from the pageDirectory.)

The querier **shall** validate its command-line arguments:

* ```pageDirectory``` is a directory produced by the crawler, and
* ```indexFilename``` is the name of a readable file.

The querier may **assume** that the input directory and files follow the designated formats.

* ```pageDirectory``` has files named 1, 2, 3, …, without gaps.
* The content of document files in ```pageDirectory``` follow the format as defined in the crawler specs; thus your code (to read a document file) need not have extensive error checking.
* The content of the file named by ```indexFilename``` follows our Index file format; thus your code (to recreate an index structure by reading a file) need not have extensive error checking.


### **Design Spec**

**User Interface**
The querier takes a pageDirectory and an indexFilename to respond to user-input queries.
An example of the command-line interface is:

```./querier pageDirectory indexFilename```

**Inputs and Outputs**

* Input: the only inputs to _querier_ are the arguments specified on the command line.
See User Interface for more information. 

* Output: _querier_ prints the result of user-input queries to standard out. 

**Functional Decomposition into Modules**
* *main*, which initializes other modules
* *indexLoad*, which loads the index file into inverted index data structure
* *querier*, which takes the query and finds the corresponding results and their scores, performing and/or logic and accessing index
* *printResults*, which takes the querier's results and prints them out with their urls and scores

Helper Modules:
* *hashtable*, which holds the index

**Pseudo Code for Logic/Algorithmic Flow**

The outline of the logic for querier is as follows:
1. Execute from a command line with usage syntax
    * ```./querier pageDirectory indexFilename```
    * where pageDirectory is directory of webpage files
    * where indexFilename is an existing index file
2. Parse command line, validate parameters, and initalize other modules
3. initialize index data structure
4. load contents of indexFilename into index data structure
5. Tokenize the query
	* Loop through the string
	* Add all words longer than length 2 to an array
	* If any nonalpha characters appear, exit the program
6. Perform the query 
	* Initialize counters struct of (docID, score) pairs
	* Loop through the words in token array
		* Until we see an 'or', loop over 'and' sequence
			* Keep a running data structure of results, intersecting its contents with each new word
		* When we see an 'or', union the total results with the and sequence just found.
	* Return query results
7. Print the query results
	* For each result, find its URL by matching its docID to its file in the pageDirectory
	* Rank results by their score
	* Print url and score of every result

**Dataflow Through Modules**

* *main* initializes other modules
* *indexLoad* loads the index file into inverted index data structure
* *querier* takes the query and finds the corresponding results and their scores, performing and/or logic and accessing index
* *printResults* takes the querier's results and prints them out with their urls and scores

**Major Data Structures**
1. Hashtable of lists of counters (representing the inverted index)
    * Lists have (word, counters) as (key, value)
    * Counters have (docID, count) as (key, count)
2. Counters Pair, which holds two counters, and facilitates intersecting the contents of counters
3. Result which has (docID, score) pair, used for holding a result. 


### **Implementation Spec**

**Function Prototypes**
/************ Primary Functionality *************/
static char **tokenize(char *query);
static void printQuery(char **tokens);
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

**Pseudo code for the components**

**Pseudo code for the functions**

**Data Structures**
1. Hashtable of lists of counters, (word, counters) as (key, value)
2. Counters Pair struct that holds pointers to to counters to facilitate intersecting
3. Result struct that holds (docID, score) of a result.

**Resource management**
Utilize count_malloc and count_free to ensure that memory is properly allocated. 

After having coded, we have confirmed through Valgrind that no memory leaks exist.

**Persistant storage**
There is no persistent storage associated with the program.

### **Testing Plan**

*Unit Testing* Test programs for each of the modules is included in the respective
files. These will all be run before integration.

*Integration Testing* Build the querier and test it's functionality. Carefully
examine results printed to stdout, making sure the format is correct,
and that the scores represented for results are correct (done by manually checking word counts of some words on some pages, and calculating score by hand).

1. Give various incorrect command line arguments. Incorrect number of arguments, 
directory that is not a proper crawler directory.

2. 