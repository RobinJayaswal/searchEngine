## **README for Tiny Search Engine (TSE)**
## Robin Jayaswal, Kyle Dotterrer, May 2016

### **Project Overview**
The Tiny Search Engine (hereafter referred to as TSE) is a basic, modular 
search engine program capabale of crawling, indexing, and querying webpages 
internal to the Dartmouth CS Department server. 

### **Compilation**
Compilation of all three project subsystems can be completed simultaneous by
a single call to ``` make ``` in the top-level tse/ directory. Here, ``` make ``` also has
the effect of building the _cs50ds_ and _common_ libraries utilized by all three
of the subsystems. Additionally, logged output processes can be added to 
both the crawler and querier subsystems by calling ``` make log ``` from the tse/
directory.  

The subsystems may also be compiled individually by calling ``` make ``` in each 
of their respective subdirectories. 

Alternatively, the script ``` tse.sh ```, located in the tse/ directory, can be
called with the usage syntax:
``` tse.sh seedUrl maxDepth ```
This script compiles all necessary components of TSE, crawls from the seedURL to
the given maxDepth, indexes the pages created during the crawling process, enters
the querier, and cleans up all files created during this process. 

### **Subsystems**

#### Crawler
_crawler_ is a standalone program that crawls webpages starting from a given
seed page out to a given maximum depth. The web pages found by the crawler are
saved to the given page directory in a specific format readable by _indexer_. 

#### Indexer
_indexer_ is a standalone program that builds an inverted index data structure
from the files in the page directory produced by _crawler_. The data structure 
is then written out to an index file in a specific format that can later be 
read and loaded into an identical data structure by _querier_. 

#### Querier
_querier_ is a standalone program that parses user-input queries and searches
for documents in the page directory that satisfy those queries. _querier_ prints
the results of the query to standard out. 

