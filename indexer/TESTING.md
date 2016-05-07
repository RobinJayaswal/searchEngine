## **TESTING for indexer subsystem**
## Robin Jayaswal, Kyle Dotterrer, May 2016

We utilized two bash scripts: indextest.sh and indexerArgsTest.sh, to test 
the functionality of our indexer program. In addition to these scripts, we
also constructed a simple directory of crawler output files with known contents
as a means of testing the index created by the indexer in a controlled 
environment. 

### _indextest.sh_

indextest.sh (provided by David Kotz) allows us to test the correctness of our
index loading and saving functions by comparing the original index file 
provided to indextest (executable) with the file that it produces. As the 
following output shows, the _diff_ command produces no ouput once the files
have been sorted into canonical order, meaning that the index file produced 
by the combination of loading and subsequent saving is identical to the input. 

**Script Output**

```
# create and save an index from files produced by crawler
[kad@flume ~/cs50/labs/tse/indexer]$ indexer ../data/crawl1 ../data/index1
# run the test script
[kad@flume ~/cs50/labs/tse/indexer]$ indextest.sh ../data/index1
running indextest to load ../data/index1 and create ../data/index1.tmp
sort ../data/index1 to create ../data/index1.sorted
sort ../data/index1.tmp to create ../data/index1.tmp.sorted
diff ../data/index1.sorted ../data/index1.tmp.sorted
[kad@flume ~/cs50/labs/tse/indexer]$ 
```

### _indexerArgsTest.sh_

indexerArgsTest.sh allows us to quickly verify that our indexer program
correctly handles the various cases of invalid arguments that can be 
provided by the user to indexer. 

**Script Output**

```
[kad@flume ~/cs50/labs/tse/indexer]$ indexerArgsTest.sh

running indexer with too few arguments:
>> indexer
Error: invalid arguments
usage: ./indexer pageDirectory indexFilename
error code: 1

running indexer with too many arguments:
>> indexer arg1 arg2 arg3
Error: invalid arguments
usage: ./indexer pageDirectory indexFilename
error code: 1

running indexer with invalid page directory:
>> indexer ../data/badDir testIndex
Error: invalid page directory
error code: 3

running indexer with valid arguments:
>> indexer ../data/crawl1 testIndex
error code: 0
```

### Controlled Indexer Execution

Finally, we created a samll test case in order to verify proper indexer behavior.
We created two new files within a subdirectory of the data directory called 
testCrawl. The content of the two files is as follows:
```
[kad@flume ~/cs50/labs/tse/data/testCrawl]$ cat 1
url 
depth
hello world
[kad@flume ~/cs50/labs/tse/data/testCrawl]$ cat 2
url
depth
hello robin kyle
```

We then ran the following command to create an index file using these two sample
files:
```
[kad@flume ~/cs50/labs/tse/indexer]$ indexer ../data/testCrawl ../data/testIndex
```

The index file produced by indexer after invocation on the sample page directory
is shown below:
```
[kad@flume ~/cs50/labs/tse/data]$ cat testIndex
hello 2 1 1 1
world 1 1
robin 2 1
kyle 2 1
```

Because the file sizes are so small, it is possible to verify visually that the
index file output matches exactly the expected output based on the two files in
the page directory. This test case therefore tells us that our indexer behaves
as expected. 






