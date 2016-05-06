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

* **main**, which parses arguments and initalizes other modules
* **index_build**, which constructs an index from the pageDirectory
* **index_save**, which writes the index to the file (see **Input/Output**)
* **index_load**, which takes an index file and create the index it describes

We also anticipate the use of helper modules providing the following data structures:
* **hashtable holding the index**

**Pseudo Code for Logic/Algorithmic Flow**

The outline of the logic for the indexer is roughly as follows:
1. 

**Dataflow Through Modules**

* **main** parses arguments and initalizes other modules
* **index_build** constructs an index from the pageDirectory
* **index_save** writes the index to the file (see **Input/Output**)
* **index_load** takes an index file and create the index it describes

**Major Data Structures**

1. Hashtable of lists of counters
    * Lists have (word, counters) as (key, value)
    * Counters have (docID, count) as (key, count)

### **Implementation Spec**

### **Testing Plan**