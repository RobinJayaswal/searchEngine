TESTING for crawler program

Robin Jayaswal, Kyle Dotterrer, 2016

### TESTING FOR CRAWLER.C

### CHECKING PAGE DIRECTORY ARGUMENT
[jay@flume ~/cs50/labs/tse]$ crawler http://old-www.cs.dartmouth.edu/~cs50/index.html dat 1
Error: invalid page directory

### CHECKING MAX AND MIN DEPTH SPECIFICATIONS
[jay@flume ~/cs50/labs/tse]$ crawler http://old-www.cs.dartmouth.edu/~cs50/index.html data 11
Error: maxDepth must be between 0-10
[jay@flume ~/cs50/labs/tse]$ crawler http://old-www.cs.dartmouth.edu/~cs50/index.html data -1
Error: maxDepth must be between 0-10

### CHECKING BAD SEED URL CHECKS
[jay@flume ~/cs50/labs/tse]$ crawler google.com data 1
Error: seedURL invalid or non-internal
[jay@flume ~/cs50/labs/tse]$ 
jay@flume ~/cs50/labs/tse]$ crawler http://old-www.cs.dartmouth.edu/~cs50/indx.html data 1
Error: Could not retrieve web page at seed url
[jay@flume ~/cs50/labs/tse]$ crawler http://www.cs.dartmouth.edu/~cs50/index.html data 1
Error: seedURL invalid or non-internal


### CHECKING FUNCTIONALITY
[jay@flume ~/cs50/labs/tse]$ crawler http://old-www.cs.dartmouth.edu/~cs50/index.html data 1
 0   Fetched: http://old-www.cs.dartmouth.edu/~cs50/index.html
 0     Saved: http://old-www.cs.dartmouth.edu/~cs50/index.html
 0  Scanning: http://old-www.cs.dartmouth.edu/~cs50/index.html
 0     Found: http://old-www.cs.dartmouth.edu/~cs50/
 0   Fetched: http://old-www.cs.dartmouth.edu/~cs50/
 0     Added: http://old-www.cs.dartmouth.edu/~cs50/
 0     Found: https://canvas.dartmouth.edu/courses/14815
 0     Found: https://piazza.com/dartmouth/spring2016/cosc05001sp16/home
 0     Found: https://gitlab.cs.dartmouth.edu
 0     Found: http://old-www.cs.dartmouth.edu/~cs50/Schedule.pdf
 0     Found: http://www.cs.dartmouth.edu/~cs50/Lectures/
 0     Found: http://www.cs.dartmouth.edu/~cs50/Reading/
 0     Found: http://www.cs.dartmouth.edu/~cs50/examples/
 0     Found: http://www.cs.dartmouth.edu/~cs50/Logistics/
 0     Found: http://www.cs.dartmouth.edu/~cs50/Resources/
 0     Found: http://www.cs.dartmouth.edu/~cs50/Labs/
 0     Found: http://www.cs.dartmouth.edu/~cs50/Project/
 0     Found: http://tools.ietf.org/html/rfc1983
 0     Found: http://www.cs.dartmouth.edu/~ccpalmer/classes/cs55/Content/Purpose.html
 0     Found: http://old-www.cs.dartmouth.edu/~dfk/
 0   Fetched: http://old-www.cs.dartmouth.edu/~dfk/
 0     Added: http://old-www.cs.dartmouth.edu/~dfk/
 1      Saved: http://old-www.cs.dartmouth.edu/~dfk/
 1      Saved: http://old-www.cs.dartmouth.edu/~cs50/
Memory Allocation Report: 13 malloc, 13 free, 0 free(NULL), 0 net


[jay@flume ~/cs50/labs/tse]$ make clean
rm -f *~
rm -f *.o
rm -f crawler
cd lib; make clean
make[1]: Entering directory '/net/tahoe3/jay/cs50/labs/tse/lib'
rm -f *~
rm -f *.o
rm -f cs50ds.a
cd memory; make clean
make[2]: Entering directory '/net/tahoe3/jay/cs50/labs/tse/lib/memory'
rm -f *~
rm -f *.o
make[2]: Leaving directory '/net/tahoe3/jay/cs50/labs/tse/lib/memory'
cd bag; make clean
make[2]: Entering directory '/net/tahoe3/jay/cs50/labs/tse/lib/bag'
rm -f *~
rm -f *.o
make[2]: Leaving directory '/net/tahoe3/jay/cs50/labs/tse/lib/bag'
cd counters; make clean
make[2]: Entering directory '/net/tahoe3/jay/cs50/labs/tse/lib/counters'
rm -f *~
rm -f *.o
make[2]: Leaving directory '/net/tahoe3/jay/cs50/labs/tse/lib/counters'
cd list; make clean
make[2]: Entering directory '/net/tahoe3/jay/cs50/labs/tse/lib/list'
rm -f *~
rm -f *.o
make[2]: Leaving directory '/net/tahoe3/jay/cs50/labs/tse/lib/list'
cd hashtable; make clean
make[2]: Entering directory '/net/tahoe3/jay/cs50/labs/tse/lib/hashtable'
rm -f *~
rm -f *.o
make[2]: Leaving directory '/net/tahoe3/jay/cs50/labs/tse/lib/hashtable'
make[1]: Leaving directory '/net/tahoe3/jay/cs50/labs/tse/lib'
cd data; rm -f *

[jay@flume ~/cs50/labs/tse]$ make
gcc -Wall -pedantic -std=c11 -ggdb -DLOG   -c -o crawler.o crawler.c
gcc -Wall -pedantic -std=c11 -ggdb -DLOG   -c -o web.o web.c
cd lib; make
make[1]: Entering directory '/net/tahoe3/jay/cs50/labs/tse/lib'
cd memory; make memory.o
make[2]: Entering directory '/net/tahoe3/jay/cs50/labs/tse/lib/memory'
gcc -Wall -pedantic -std=c11 -ggdb   -c -o memory.o memory.c
make[2]: Leaving directory '/net/tahoe3/jay/cs50/labs/tse/lib/memory'
cd list; make list.o
make[2]: Entering directory '/net/tahoe3/jay/cs50/labs/tse/lib/list'
gcc -Wall -pedantic -std=c11 -ggdb   -c -o list.o list.c
make[2]: Leaving directory '/net/tahoe3/jay/cs50/labs/tse/lib/list'
cd hashtable; make hashtable.o
make[2]: Entering directory '/net/tahoe3/jay/cs50/labs/tse/lib/hashtable'
gcc -Wall -pedantic -std=c11 -ggdb   -c -o hashtable.o hashtable.c
make[2]: Leaving directory '/net/tahoe3/jay/cs50/labs/tse/lib/hashtable'
gcc -Wall -pedantic -std=c11 -ggdb   -c -o hashtable/jhash.o hashtable/jhash.c
gcc -Wall -pedantic -std=c11 -ggdb   -c -o hashtable/list.o hashtable/list.c
cd bag; make bag.o
make[2]: Entering directory '/net/tahoe3/jay/cs50/labs/tse/lib/bag'
gcc -Wall -pedantic -std=c11 -ggdb   -c -o bag.o bag.c
make[2]: Leaving directory '/net/tahoe3/jay/cs50/labs/tse/lib/bag'
cd counters; make counters.o
make[2]: Entering directory '/net/tahoe3/jay/cs50/labs/tse/lib/counters'
gcc -Wall -pedantic -std=c11 -ggdb   -c -o counters.o counters.c
make[2]: Leaving directory '/net/tahoe3/jay/cs50/labs/tse/lib/counters'
ar cr cs50ds.a memory/memory.o list/list.o hashtable/hashtable.o hashtable/jhash.o hashtable/list.o bag/bag.o counters/counters.o
make[1]: Leaving directory '/net/tahoe3/jay/cs50/labs/tse/lib'
gcc -Wall -pedantic -std=c11 -ggdb -DLOG crawler.o web.o lib/cs50ds.a -lcurl -o crawler
[jay@flume ~/cs50/labs/tse]$ crawler http://old-www.cs.dartmouth.edu/~kotz/tse data 0
 0   Fetched: http://old-www.cs.dartmouth.edu/~kotz/tse
 0     Saved: http://old-www.cs.dartmouth.edu/~kotz/tse
Memory Allocation Report: 5 malloc, 5 free, 0 free(NULL), 0 net

[jay@flume ~/cs50/labs/tse]$ crawler http://old-www.cs.dartmouth.edu/~kotz/tse data 1
 0   Fetched: http://old-www.cs.dartmouth.edu/~kotz/tse
 0     Saved: http://old-www.cs.dartmouth.edu/~kotz/tse
 0  Scanning: http://old-www.cs.dartmouth.edu/~kotz/tse
 0     Found: http://old-www.cs.dartmouth.edu/~kotz/tse/
 0   Fetched: http://old-www.cs.dartmouth.edu/~kotz/tse/
 0     Added: http://old-www.cs.dartmouth.edu/~kotz/tse/
 1      Saved: http://old-www.cs.dartmouth.edu/~kotz/tse/

[jay@flume ~/cs50/labs/tse]$ crawler http://old-www.cs.dartmouth.edu/~kotz/tse/ data 1
 0   Fetched: http://old-www.cs.dartmouth.edu/~kotz/tse/
 0     Saved: http://old-www.cs.dartmouth.edu/~kotz/tse/
 0  Scanning: http://old-www.cs.dartmouth.edu/~kotz/tse/
 0     Found: http://old-www.cs.dartmouth.edu/~kotz/tse/wiki/Computer_science.html
 0   Fetched: http://old-www.cs.dartmouth.edu/~kotz/tse/wiki/Computer_science.html
 0     Added: http://old-www.cs.dartmouth.edu/~kotz/tse/wiki/Computer_science.html
 0     Found: http://old-www.cs.dartmouth.edu/~kotz/tse/wiki/C_(programming_language).html
 0   Fetched: http://old-www.cs.dartmouth.edu/~kotz/tse/wiki/C_(programming_language).html
 0     Added: http://old-www.cs.dartmouth.edu/~kotz/tse/wiki/C_(programming_language).html
 0     Found: http://old-www.cs.dartmouth.edu/~kotz/tse/wiki/Unix.html
 0   Fetched: http://old-www.cs.dartmouth.edu/~kotz/tse/wiki/Unix.html
 0     Added: http://old-www.cs.dartmouth.edu/~kotz/tse/wiki/Unix.html
 0     Found: http://old-www.cs.dartmouth.edu/~kotz/tse/wiki/Dartmouth_College.html
 0   Fetched: http://old-www.cs.dartmouth.edu/~kotz/tse/wiki/Dartmouth_College.html
 0     Added: http://old-www.cs.dartmouth.edu/~kotz/tse/wiki/Dartmouth_College.html
 0     Found: http://old-www.cs.dartmouth.edu/~kotz/tse/wiki/Hash_table.html
 0   Fetched: http://old-www.cs.dartmouth.edu/~kotz/tse/wiki/Hash_table.html
 0     Added: http://old-www.cs.dartmouth.edu/~kotz/tse/wiki/Hash_table.html
 0     Found: http://old-www.cs.dartmouth.edu/~kotz/tse/wiki/Linked_list.html
 0   Fetched: http://old-www.cs.dartmouth.edu/~kotz/tse/wiki/Linked_list.html
 0     Added: http://old-www.cs.dartmouth.edu/~kotz/tse/wiki/Linked_list.html
 1      Saved: http://old-www.cs.dartmouth.edu/~kotz/tse/wiki/Linked_list.html
 1      Saved: http://old-www.cs.dartmouth.edu/~kotz/tse/wiki/Hash_table.html
 1      Saved: http://old-www.cs.dartmouth.edu/~kotz/tse/wiki/Dartmouth_College.html
 1      Saved: http://old-www.cs.dartmouth.edu/~kotz/tse/wiki/Unix.html
 1      Saved: http://old-www.cs.dartmouth.edu/~kotz/tse/wiki/C_(programming_language).html
 1      Saved: http://old-www.cs.dartmouth.edu/~kotz/tse/wiki/Computer_science.html
Memory Allocation Report: 29 malloc, 29 free, 0 free(NULL), 0 net

[jay@flume ~/cs50/labs/tse]$ crawler http://old-www.cs.dartmouth.edu/~kotz/tse data 2
 0   Fetched: http://old-www.cs.dartmouth.edu/~kotz/tse
 0     Saved: http://old-www.cs.dartmouth.edu/~kotz/tse
 0  Scanning: http://old-www.cs.dartmouth.edu/~kotz/tse
 0     Found: http://old-www.cs.dartmouth.edu/~kotz/tse/
 0   Fetched: http://old-www.cs.dartmouth.edu/~kotz/tse/
 0     Added: http://old-www.cs.dartmouth.edu/~kotz/tse/
 1      Saved: http://old-www.cs.dartmouth.edu/~kotz/tse/
 1   Scanning: http://old-www.cs.dartmouth.edu/~kotz/tse/
 1      Found: http://old-www.cs.dartmouth.edu/~kotz/tse/wiki/Computer_science.html
 1    Fetched: http://old-www.cs.dartmouth.edu/~kotz/tse/wiki/Computer_science.html
 1      Added: http://old-www.cs.dartmouth.edu/~kotz/tse/wiki/Computer_science.html
 1      Found: http://old-www.cs.dartmouth.edu/~kotz/tse/wiki/C_(programming_language).html
 1    Fetched: http://old-www.cs.dartmouth.edu/~kotz/tse/wiki/C_(programming_language).html
 1      Added: http://old-www.cs.dartmouth.edu/~kotz/tse/wiki/C_(programming_language).html
 1      Found: http://old-www.cs.dartmouth.edu/~kotz/tse/wiki/Unix.html
 1    Fetched: http://old-www.cs.dartmouth.edu/~kotz/tse/wiki/Unix.html
 1      Added: http://old-www.cs.dartmouth.edu/~kotz/tse/wiki/Unix.html
 1      Found: http://old-www.cs.dartmouth.edu/~kotz/tse/wiki/Dartmouth_College.html
 1    Fetched: http://old-www.cs.dartmouth.edu/~kotz/tse/wiki/Dartmouth_College.html
 1      Added: http://old-www.cs.dartmouth.edu/~kotz/tse/wiki/Dartmouth_College.html
 1      Found: http://old-www.cs.dartmouth.edu/~kotz/tse/wiki/Hash_table.html
 1    Fetched: http://old-www.cs.dartmouth.edu/~kotz/tse/wiki/Hash_table.html
 1      Added: http://old-www.cs.dartmouth.edu/~kotz/tse/wiki/Hash_table.html
 1      Found: http://old-www.cs.dartmouth.edu/~kotz/tse/wiki/Linked_list.html
 1    Fetched: http://old-www.cs.dartmouth.edu/~kotz/tse/wiki/Linked_list.html
 1      Added: http://old-www.cs.dartmouth.edu/~kotz/tse/wiki/Linked_list.html
 2       Saved: http://old-www.cs.dartmouth.edu/~kotz/tse/wiki/Linked_list.html
 2       Saved: http://old-www.cs.dartmouth.edu/~kotz/tse/wiki/Hash_table.html
 2       Saved: http://old-www.cs.dartmouth.edu/~kotz/tse/wiki/Dartmouth_College.html
 2       Saved: http://old-www.cs.dartmouth.edu/~kotz/tse/wiki/Unix.html
 2       Saved: http://old-www.cs.dartmouth.edu/~kotz/tse/wiki/C_(programming_language).html
 2       Saved: http://old-www.cs.dartmouth.edu/~kotz/tse/wiki/Computer_science.html
Memory Allocation Report: 33 malloc, 33 free, 0 free(NULL), 0 net

### CHECK THE OUTPUT IS IN RIGHT FORM
[jay@flume ~/cs50/labs/tse]$ cd data
[jay@flume ~/cs50/labs/tse/data]$ ls
1  2  3  4  5  6  7  8
[jay@flume ~/cs50/labs/tse/data]$ cat 8
http://old-www.cs.dartmouth.edu/~kotz/tse/wiki/Computer_science.html
2
<!DOCTYPE html>
<html lang="en" dir="ltr" class="client-nojs">

<!-- Mirrored from en.wikipedia.org/wiki/Computer_science by HTTrack Website Copier/3.x [XR&CO'2013], Sat, 29 Mar 2014 23:53:48 GMT -->
<!-- Added by HTTrack --><meta http-equiv="content-type" content="text/html;charset=UTF-8" /><!-- /Added by HTTrack -->
<head>
<meta charset="UTF-8" />

...

tchToken":false});},{},{});
/* cache key: enwiki:resourceloader:filter:minify-js:7:868e43d07ede2616d2d1dc3507cd8145 */
}</script>
<script>if(window.mw){



