# CS50 Labs 4-5-6: Tiny Search Engine

## Robin Jayaswal, Kyle Dotterrer


### **Overview**

The crawler is a subsystem of the TSE. Crawler crawls a website and retrieves webpages starting with a specified URL. It parses the initial webpage, extracts any embedded href URLs and retrieves those pages, and crawls the pages found at those URLs. The crawler limits itself to maxDepth hops from the seed URL. When the crawling process is complete, the indexing of the collected documents can begin.


### **Program Compilation**

Within the tse/ directory, the default crawler can be compiled with the command: **_make_**

Specifying the following command line will display output of Crawler processes, as well as print out a memory allocation report at the termination of the program: **_make log_**


### **Exit Codes:**

* 0: Success

#### Argument Errors, 0-10

* 1: Incorrect Argument Count
* 3: seedURL either invalid or not internal
* 4: unable to retrieve page at seed url
* 5: pageDirectory is not writeable
* 7: maxDepth not an integer
* 8: maxDepth outside of range (0 through 10, inclusive)