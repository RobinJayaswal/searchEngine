## **README for querier subsystem**
## Robin Jayaswal, Kyle Dotterrer, May 2016

### **Overview**
Querier is a subsystem of TSE. 

### **Program Compilation**
Within the querier/ subdirectory of TSE, the querier program
can be compiled with a single command: 

```make```

Additionally, we implement memory allocation and deallocation
trackign throughout our program. The results of this tracking can 
be displayed by way of an alternate compilation process invoked by the command:

```make log```

The equivalent commands issued from the top level (tse/) directory 
build all three subsystems (with or without logged process output) 
as well as the necessary libraries. 

### **Usage**
Querier is executed with the syntax:
```querier pageDirectory indexFilename```

Where:
* pageDirectory is the path to a directory containing valid text files
produced by crawler during the crawling process; pageDirectory must 
contain special file .crawler in order to be considered valid by querier
* indexFilename is the name of the index file created by indexer during
the indexing process


### **Assumptions**
* pageDirectory must contain the special file .crawler (content not considered)
in order for the directory to be considered valid by querier
* pageDirectory has files numbered 1, 2, 3, ... without gaps
* the content of files in pageDirectory follows the format specified in the 
crawler specifications; 
* we thus implement little error checking when loading
the index file into an inverted index data structure
* the strings 'and' and 'or' are treated as operators by querier; thus these
two words cannot be queried for
* the query '_andsequence1_ or _andsequence1', where the user input _andsequences_ 
are equivalent on either side of an 'or' operator, is considered valid; the results
returned from such a query are still in the correct sorted order, however the scores
for each document are doubled to account for multiple satisfactions of the _andsequence_.
this result generalizes to an arbitrary number of equivalent _andsequences_
* the _pageDirectory_ and _indexFilename_ are assumed to be compatible once the arguments
passed to _querier_ are validated; the program informs the user and exits if the 
_pageDirectory_ and _indexFilename_ are found to be invalid while printing results


### **Limitations**
* the scoring criteria utilized by _querier_ is not entirely robust: 
because the score for an _andsequence_ is defined to be the minimum
score among each of the individual words contained within it, this 
scoring scheme is far from optimal. A more advanced program would 
consider a weighted average of all terms within the _andsequence_ when
calculating the score rather than using the minimum


### **Error Codes**
* 0: Successful excecution

#### Argument Errors, 1-10
* 1: invalid number of arguments
* 3: unable to load index from indexFilename
* 5: invalid or unreadable index file passed as argument
* 7: invalid page directory passed as argument

#### Invalid Memory Usage
* 99: unexpected NULL pointer passed as function argument


