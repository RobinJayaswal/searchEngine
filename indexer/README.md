## README for indexer subsystem (CS50 Lab 5)
## Robin Jayaswal, Kyle Dotterrer, May 2016

### **Overview**
The indexer is a subsystem of TSE. Indexer builds an inverted index data 
structure from a collection of text files output by the crawling process. These
files are located in the page directory passed to the indexer. Once all of the
files have been scanned and the directory has been built, indexer write the 
data structure to a file with a user-provided name in a specific format. See
DESIGN for additional information.

In addition to the indexer program, the indexer/ directory contains an 
implementation files for the indextest program. Indextest loads a previously
created index file into an inverted index data structure, and subsequently 
writes this inverted index out to another index file. 

### **Program Compilation**
Within the indexer/ subdirectory of TSE, both the indexer and indextest programs
can be compiled with a single command: 

_make_

### **Usage**
#### _Indexer_
indexer pageDirectory indexFilename

Where:
* pageDirectory is the path to the directory containing webpage files created 
by crawler
* indexFilename is the name of the file to which the index will be written

#### _Indextest_
indextest oldIndexFilename newIndexFilename

Where:
* oldIndexFilename is the name of the existing index file which will be loaded
into an inverted index data structure by indextest
* newIndexFilename is the name of the new file that will be created by 
indextest, and will contain the newly written index

### **Imported Modules / Linked Libraries**
* The hashtable module (Courtesy of Kyle Dotterrer) provides the data structure 
used to implement the inverted index (See module README for additional 
information). Module linked via _cs50ds.a_ library. 
* The counters module (Courtesy of Kyle Dotterrer) provides the data structure
utilized by the hashtable to maintain the inverted index for each individual 
word (See module README for additional information). Module linked via 
_cs50ds.a_ library. 
* The file module (Courtesy of David Kotz) provides functions to read file
contents into the program in manageable formats. Module linked via _common.a_
library. 
* The word module (Courtesy of David Kotz) provides functions to read and
normalize words from a file. Module linked via _common.a_ library. 
* The index module provides the functions for loading the inverted index from a
file and saving an inverted index to a file. Module linked via _common.a_ 
library. 

### **Assumptions**
#### Indexer
* pageDirectory has files named 1, 2, 3, …, without gaps.
* The content of files in pageDirectory follow the format as defined in the 
crawler specs. 

#### Indextest
* The content of the index file follows the format specified in the 
requirements specifications. 

### **Limitations**

### **Exit Codes**
* 0: Success

#### Argument Errors, 1-10
* 1: invalid number of arguments
* 3: invalid file/directory specified (pageDirectory in indexer; 
oldIndexFilename in indextest)

#### Allocation Failures, 11-20
* 11: data structure creation failure

