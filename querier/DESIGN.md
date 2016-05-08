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

**Pseudo Code for Logic/Algorithmic Flow**

The outline of the logic for querier is as follows:
1. Execute from a command line with usage syntax
    * ```./querier pageDirectory indexFilename```
    * where pageDirectory is directory of webpage files
    * where indexFilename is an existing index file
2. Parse command line, validate parameters, and initalize other modules
3. initialize index data structure
4. load contents of indexFilename into index data structure

**Dataflow Through Modules**

* *main* initializes other modules
* *indexLoad* loads the index file into inverted index data structure

**Major Data Structures**
1. Hashtable of lists of counters (representing the inverted index)
    * Lists have (word, counters) as (key, value)
    * Counters have (docID, count) as (key, count)


### **Implementation Spec**

* *parseArguments* parses command line arguments

### **Testing Plan**

