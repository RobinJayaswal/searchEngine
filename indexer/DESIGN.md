## DESIGN for indexer subsystem
## Robin Jayaswal, Kyle Dotterrer, May 2016

### **Requirements Spec**

The TSE indexer is a standalone program that reads the document files produced by the TSE crawler, builds an index, and writes that index to a file. Its companion, the index tester, loads an index file produced by the indexer and saves it to another file.

The indexer **shall**:

* execute from a command line with usage syntax ./indexer pageDirectory indexFilename \
where pageDirectory is a directory produced by the crawler, and \
where indexFilename is the name of a file into which the index should be written.
* read documents from the pageDirectory, each of which has a unique document ID,
wherein the document id starts at 1 and increments for each new document,
and the filename is of form pageDirectory/id,
and the first line of the file is the URL,
and the second line of the file is the depth,
and the rest of the file is the page content.
* build an inverted-index data structure mapping from words to (documentID, count) pairs, wherein each count represents the number of occurrences of the given word in the given document. Ignore words with fewer than three characters, and NormalizeWord before indexing.
* create a file indexFilename and write the index to that file, in the format described below.

The indexer **shall** validate its command-line arguments:

* pageDirectory is a directory produced by the crawler (see below), and
* indexFilename is the name of a writeable file.

The indexer *may* assume that

* pageDirectory has files named 1, 2, 3, …, without gaps.
* The content of files in pageDirectory follow the format as defined in the crawler specs; thus your code (to create a WebPage by reading a file) need not have extensive error checking.

The index tester **shall**:

* execute from a command line with usage syntax ./indextest oldIndexFilename newIndexFilename
where oldIndexFilename is the name of a file produced by the indexer, and
where newIndexFilename is the name of a file into which the index should be written.
* load the index from the oldIndexFilename into an inverted-index data structure.
* create a file newIndexFilename and write the index to that file, in the format described below.

It need not validate its command-line arguments other than to ensure that it receives precisely two arguments; it may simply try to open the oldIndexFilename for reading and, later, try to open the newIndexFilename for writing. You may use indextest.sh (provided) to verify that the output is identical to (or equivalent to) the input.

The index tester *may* assume that

* The content of the index file follows the format specified below; thus your code (to recreate an index structure by reading a file) need not have extensive error checking.

### **Design Spec**

**User Interface**

The indexer takes a pageDirectory and creates an index file.
An example of the command-line interface is:

```./indexer pageDirectory indexFilename```

**Inputs and Outputs**

* Input: The only inputs are the parameters passed on the commadn line. 
See User interface

* Output: Writes the index to the file specified by indexFilename parameter (see **User Interface**).
The file contains one line for every word in the index. On that same line will
be printed a (docID, count) pair for every page on which that word appeared.
The format of the file is therefore as follows:

word1 docID count [docID count]...
word2 docID count [docID count]...
...
wordn docID count [docID count]...

**Functional Decomposition into Modules**

We anticipate the following modules or functions:

* *main*, which parses arguments and initalizes other modules
* *index_build*, which constructs an index from the pageDirectory
* *index_save*, which writes the index to the file (see **Input/Output**)
* *index_load*, which takes an index file and create the index it describes

We also anticipate the use of helper modules providing the following data structures:
* **hashtable holding the index**

**Pseudo Code for Logic/Algorithmic Flow**

The outline of the logic for the indexer is roughly as follows:
1. Execute from a command line with usage syntax
    * ```./indexer pageDirectory indexFilename```
    * where pageDirectory is directory of webpage files
    * where indexFilename is the file to write the index to
2. Parse command line, validate parameters, and initalize other modules
3. Initialize index data structure
4. While there are more webpage files in pageDirectory to index:
    1. Get the filename of the next file to read
    2. Open the file
    3. Read html of that webpage
    4. While there are words left in the html
        1. If word not in index, add to index
        2. Increment count of (docID, count) pair for the word and the current page
5. Use *index_save* to save the index to indexFile
    1. While there are words in index:
        1. For each word, print word to line
        2. While there are more pages containing that word, write (docID, count) pair to same line

**Dataflow Through Modules**

* *main* parses arguments and initalizes other modules
* *index_build* constructs an index from the pageDirectory
* *index_save* writes the index to the file (see **Input/Output**)
* *index_load* takes an index file and create the index it describes

**Major Data Structures**

1. Hashtable of lists of counters
    * Lists have (word, counters) as (key, value)
    * Counters have (docID, count) as (key, count)

### **Implementation Spec**

**Function Prototypes**
*Indexer*

Exported:

Opaque:
    *static void indexBuild(char *pageDir, hashtable_t *index);
    *static int parseArguments(const int argc, char *argv[]);
    *static bool isCrawlerDirectory(char *dir);
    *int numDigits(int number);
    *static void hashDeleteFunc(void *data);

*Index*

Exported:
    *void indexSave(char *indexFile, hashtable_t *index);
    *int indexLoad(char *indexFile, hashtable_t *index);

Opaque:
    *static void indexWrite(void *arg, char *key, void *data);
    *static void printCounterPair(void *arg, int key, int count);


**Pseudo code for the components**

* *main(const int argc, char\* argv[])*
    * if (parseArguments(argc, argv)) returns nonzero) { exit with error }
    * else { continue with program }
    * hashtable index == hashtable_new(size, hashDeleteFunc)
    * indexBuild(argument1-pageDir, index)
    * indexSave(argument2-indexFilename, index)
    * hashtable_delete(index)
    * exit

* *int parseArguments(argc, argv)*
    * if argc != 3 return nonzero
    * if isCrawlerDirectory(argv[1]) == false, return nonzero

* *void indexBuild(char \*pageDir, hashtable_t \*index)*
    * int docID = 1
    * char *filename = malloc
    * sprintf(filename, "%s/%i", pageDir, docID)
    * while(fopen can open file with current docID)
        * fp = fopen(filename)
        * html = file2string(f)
        * while(GetNextWord gets a word from html)
            * NormalizeWord(word)
            * if (strlen(word) > 2)
                * if(hashtable_find(index, word) returns counters) {
                1. increment counter in (docID, counter) pair }
                * else {
                1. counters = new_counter
                2. counters_add(counters, docID, 1)
                3. hashtable_insert(index, word, counters) }
            * free(word)
        * close(file), free(html), free(filename)
        * docID++
        * fn = malloc, sprintf(filename, "%s/%i", pageDir, docID)

* * void indexSave(char \*indexFilename, hashtable_t \*index))*
    * FILE *fp = fopen(indexFilename, "w")
    * hashtable_iterate(index, indexWrite), fp);
    * close(fp)

* * int indexLoad(char *indexFile, hashtable_t *index)
    * fp = open(indexFile, "w")
    * while(fscan(fp, "%s ", word) is successful)
        * counters_t *counters = counters_new()
        * hashtable_insert(index, word, counters)
        * while (fscanf(fp, " %d %d", docID, count) is successful)
            * counters_set(counters, docID, count);

**Pseudo code for the functions**

* *bool isCrawlerDirectory(char \*dir)*
    * char *fn = malloc
    * sprintf(fn, "%s/.crawler", dir)
    * if (fopen opens file with naem fn) { return true }
    * else { return false }
    * fclose(file), free(fn)

* *int numDigits(int number)*
    * if (number has one digit) { return 1 }
    * else { return 1 + numDigits(number without first digit)}

* *static void hashDeleteFunc(void \*data)*
    * counters_t counters = data
    * counters_delete(counters)

* *void indexWrite(void \*arg, char \*key, void \*data)*
    * FILE *file = arg;
    * char *word = key;
    * counters_t *counters = data;
    * fprintf(fp, "%s", word);
    * counters_iterate(counters, printCounterPair, fp);
    * fprintf(fp, "\n");
    }
}

* *void printCounterPair(void \*arg, int key, int count)*
    * fprintf(fp, " %d %d", docID, count)

**Data Structures**
1. Hashtable of lists of counters, (word, counters) as (key, value)
2. Lists, (word, counters) as (key, value)
3. Counters, (docID, count) as (key, count)

**Resource management**
Utilize count_malloc and count_free to ensure that memory is properly allocated. 

After having coded, we have confirmed through Valgrind that no memory leaks exist.

**Persistant storage**

Index is stored in file. Filename to be written to is provided as second argument on command line.


### **Testing Plan**

*Unit Testing* Test programs for each of the modules is included in the respective
files. These will all be run before integration.

*Integration Testing* Build the indexer and test it's functionality. Carefully
examine output files that index is written to, making sure the format is correct,
and that the word counts represented on the pages are correct (done by manually checking
word counts of some words on some pages, and comparing). Make sure no words are duplicated.

1. Give various incorrect command line arguments. Incorrect number of arguments, 
directory that is not a proper crawler directory.

2. Run scripts to ensure that when index is loaded from index file, then written
out again to a new index file, the two index files match.

