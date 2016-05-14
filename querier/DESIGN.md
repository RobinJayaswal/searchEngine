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

_Primary Functionality_
* static char **tokenize(char *query);
* static void printQuery(char **tokens);
* static bool validateQuery(char **tokens);
* static counters_t *performQuery(char **tokens, hashtable_t *index);
* static result_t **sortResults(counters_t *results, int *numResults);
* static void printResults(result_t **results, int numResults, char *pageDir);


_Counters Helpers_
* static counters_t *intersectCounters(counters_t *ctrs1, counters_t *ctrs2);
* static void intersectHelper(void *arg, int key, int count);
* static void unionCounters(counters_t *ctrs1, counters_t *ctrs2);
* static void unionHelper(void *arg, int key, int count);
* static void countCounters(void *arg, int key, int count);
* static void insertCounter(void *arg, int key, int count);


_Comparison Functions_
* static int resultCompare(const void *a, const void *b);

_Memory Deallocation_
* static void freeTokensArray(char **tokens);
* static void freeResults(result_t **results, int len);

_Auxillary Functions_
* static int parseArguments(const int argc, char *argv[]);
* static bool isCrawlerDirectory(char *dir);
* static int linesInFile(char *fn);
* static int numDigits(int number);
* static int arrayLength(char **array);
* static void hashDeleteFunc(void *data);
* static void *checkPtr(void *ptr, char *loc);

**Pseudo code for the components**
* *main(const int argc, char\* argv[])*
    * if (parseArguments(argc, argv)) returns nonzero) { exit with error }
    * else { continue with program }
    * initialize index 
    * load index from indexFile
    * while (user does not input EOF command):
        * tokenize the query
        * print the clean query
        * check the query for invalid constructions
        * perform the query and return counters object representing results
        * sort the results
        * print the results
    * clean up data structures 
    * exit

* *tokenize(char *query)*
    * allocate space for array of string tokens
    * use whitespace as delimeter
    * while there are still more tokens in the string:
        * if the number of words in tokens array will exceed current array size:
            * reallocate more space for array
        * for each character in the current word:
            * if character is non alpha-numeric:
                * print error message
                * deallocate array
                * exit function
            * else: 
                * make letter lowercase
        * copy the cleaned word into tokens array
    * allocate space for string representation of array length
    * build string representation of array length
    * store array length string at position 0 in tokens array
    * free the original array length string
    * exit
    
* *printQuery(char **tokens)*
    * print "query " to stdout
    * for each word in tokens array:
        * print the word to stdout
        * add a trailing space
    * add a trailing newline
    
* *validateQuery(char **tokens)*
    * get the tokens array length
    * if first word in query is 'and' or 'or'
        * print error message
        * return false
    * else if last word in query is 'and' or 'or'
        * print error message
        * return false
    * for every word in tokens array:
        * if current word is 'and' or 'or'
            * if previous word was 'and' or 'or'
                * return false
            * else mark current word as operator
        * else mark current word as non-operator
    * return true
    
* *performQuery(char **tokens, hashtable_t *index)*
    * get array length from first position of tokens array
    * initialize new counters for final results
    * for every word in the tokens array:
        * initialize an _andsequence_ with that word
        * while we are still within bounds of array and 'or' is not seen:
            * if 'and' is seen
                * increment one position forward in array
            * else:
                * find docs with current word
                * intersect current _andsequence_ with results for new word
                * increment one position forward in array
        * union the _andsequence_ with the current final results
        * delete the _andsequence_
        * increment one position forward in array
    * return results
   
* *sortResults(counters_t *results, int numResults)*
    * iterate over results to count the number of results
    * allocate memory for new array of pointers to results structs
    * iterate over results, inserting new results struct into array for each counter seen
    * sort the results array
    * return the sorted array
    
* *printResults(result_t **sorted, int numResults, char *pageDir)*
    * if number of results is 0
        * print "no documents match"
    * else:
        * print results header with number of satisfying documents
        * for each result in the sorted array of results:
            * allocate space for filename of result's page
            * build the filename from page directory and doc ID
            * open the file corresponding to current result
            * read the result's url into variable
            * print the score, document, and url information
            * free the filename
            * close the file
            * free the built url
        * print individual query footer to mark end of results

**Pseudo code for the functions**

* *inserectCounters(counters_t *ctrs1, counters_t *ctrs2)*
    * create new counters object to represent intersection
    * allocate space for new countersPair struct
    * fill countersPair with empty itersection and ctrs2
    * iterate over ctrs1, intersecting both original counters into new intersection
    * free the pair structure
    * return result of intersection
    
* *intersectHelper(void *arg, int key, int count)*
    * cast argument to counters type
    * get the key from ctrs2
    * if the current key in ctrs1 is also in ctrs2:
        * sum the counts and add sum to insection with key
        
* *unionCounters(counters_t *ctrs1, counters_t *ctrs2)*
    * iterate over ctrs2, unioning in place with ctrs1

* *unionHelper(void *arg, int key, int count)*
    * cast argument to counters type
    * calculate sum of current count from ctrs2 and ctrs1
    * set the value of current key in ctrs1 to sum
    
* *countCounters(void *arg, int key, int count)*
    * cast argument to integer
    * increment the argument by 1
    
* *insertCounter(void *arg, int key, int count)*
    * initialize static variable index = 0
    * cast argument to results array
    * allocate space for new result entry
    * set the key and value of struct to current counter key and value
    * insert the pointer to new struct into results array
    * increment static index variable
    
* *freeTokensArray(char **tokens)*
    * get array length from position 0 of tokens array
    * for every element in the array:
        * if the element is not a null pointers:
            * free the element
    * free the length of tokens array stored at position 0
    * free the entire tokens aray
    
* *freeResults(result_t **results, int len)*
    * for every element in results array:
        * if the pointer is not null:
            * free the element
    * free the entire array
    
* *parseArguments(const int argc, char *argv[])*
    * if number of command line arguments is not 3:
        * print error message
        * return nonzero status
    * if pageDirectory is not crawler directory:
        * print error message
        * return nonzero status
    * if index file is invalid or unreadable:
        * print error message
        * return nonzero status
    * return 0 status
    
* *isCrawlerDirectory(char *dir)*
    * allocate space for the filename
    * build filename string from page directory and '.crawler'
    * if fail to open '.crawler' file in page directory
        * return false
    * else:
        * return true
        
* *linesInFile(char *fn)*
    * open the file 
    * rewind file pointer to beginning of file
    * initialize number of newlines seen to 0
    * while EOF is not reached:
        * if newline is seen:
            * increment number of newlines
    * return number of newlines seen
    
* *numDigits(int number)*
    * if number is 1 digit:
        * return 1
    * else:
        * recursively call numDigits, adding 1 to result and dividing number by 10
        
* *arrayLength(char **array)*
    * get length string from first position in array
    * cast string to long integer representation
    * return integer representation
    
* *hashDeleteFunc(void *data)*
    * cast data argument to counters type
    * call counters delete to delete entire data structure

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