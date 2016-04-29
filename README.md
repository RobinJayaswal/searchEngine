# CS50 Labs 4-5-6: Tiny Search Engine

## Robin Jayaswal, Kyle Dotterrer

See [Lab 4 instructions](http://www.cs.dartmouth.edu/~cs50/Labs/Lab4.html)
for more information.

### **Program Overview**
Tiny Search Engine (TSE) is a basic search engine capable of crawling, indexing, and querying internal web pages.

#### TSE consists of three subsystems:
* Crawler
* Indexer
* Querier 


### **Design Specifications**

#### Crawler:
The crawler is a subsystem of the TSE. Crawler crawls a website and retrieves webpages starting with a specified URL. It parses the initial webpage, extracts any embedded href URLs and retrieves those pages, and crawls the pages found at those URLs. The crawler limits itself to maxDepth hops from the seed URL. When the crawling process is complete, the indexing of the collected documents can begin.


### **Exit Codes:**

* 0: Success

#### Argument Errors, 0-10

* 1: Incorrect Argument Count
* 3: seedURL either invalid or not internal
* 4: unable to retrieve page at seed url
* 5: pageDirectory is not writeable
* 7: maxDepth not an integer
* 8: maxDepth outside of range (0 through 10, inclusive)