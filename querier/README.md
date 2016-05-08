## **README for querier subsystem**
## Robin Jayaswal, Kyle Dotterrer, May 2016

### **Overview**
Querier is a subsystem of TSE. 

### **Program Compilation**
Within the querier/ subdirectory of TSE, the querier program
can be compiled with a single command: 

_make_

The equivalent command issued from the top level (tse/) directory 
builds all three subsystems as well as necessary libraries. 

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


### **Limitations**


### **Error Codes**
* 0: Successful excecution

#### Argument Errors, 1-10
* 1: invalid number of arguments
* 3: unable to load index from indexFilename
* 5: invalid or unreadable index file given as argument
* 7: invalid page directory given as argument


