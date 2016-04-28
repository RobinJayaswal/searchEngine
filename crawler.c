/* ========================================================================= */
/* 
 * crawler.c - crawl internal webpages, saving to specified page directory
 * 
 * usage: crawler seedPage pageDirectory maxDepth
 *        (all arguments non-optional)
 * output: 
 * 
 * functions:
 * 		> pagesave:
 * 		> pagescan: 
 *      > webpageNew:
 *      > webpageDelete:
 *      > isWritableDirectory:
 *      > numDigits:
 *      > htDeleteFunc: 
 * stdin:
 * stdout:
 * stderr: error messages
 *
 * Robin Jayaswal, Kyle Dotterrer, April 2016
 */
 /* ======================================================================== */

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <unistd.h>
#include "lib/memory/memory.h"
#include "lib/bag/bag.h"
#include "lib/hashtable/hashtable.h"
#include "web.h"

char* MALLOC_ERROR = "Error: memory allocation error";   // error message

bool pagesave(WebPage *page, char *pageDr);
bool pagescan(WebPage *page, bag_t *pageBag, hashtable_t *urlTable);
WebPage *webpageNew(char *url, int depth);
void webpageDelete(void *webpage);
bool isWritableDirectory(char *dir);
int numDigits(int number);
int putInt(int num, FILE *fp);
void htDeleteFunc(void *data);
//inline static void log(const char *word, const int depth, const char *url);

int main(const int argc, char *argv[])
{
	/* Begin By Checking All Arguments */

	char *progName = argv[0];

	if (argc != 4) {
		fprintf(stderr, "Error: invalid arguments\n");
		fprintf(stderr, "usage: %s seedURL pageDirectory maxDepth\n", progName);
		exit(1);
	}

	/* Test Format/Validity of Arguments */
	char *seedURL = argv[1];
	char *pageDirectory = argv[2];
	int maxDepth;

	if (!IsInternalURL(seedURL)) {
		fprintf(stderr, "Error: seedURL invalid or non-internal\n");
		exit(3);
	}

	if (!isWritableDirectory(pageDirectory)) {
		fprintf(stderr, "Error: invalid page directory\n");
		exit(5);
	}

	if (sscanf(argv[3], "%i", &maxDepth) < 1) {
		fprintf(stderr, "Error: maxDepth must be integer\n");
		exit(7);
	}
	if (maxDepth < 0 || maxDepth > 10) {
		fprintf(stderr, "Error: maxDepth must be between 0-10\n");
		exit(8);
	}

	/* Argument Tests Passed. Initialize Data Structures */

	bag_t *pageBag = bag_new(webpageDelete);
	hashtable_t *urlTable = hashtable_new(100, htDeleteFunc);

	// create WebPage struct for initial seed url page 
	WebPage *seedPage;
	seedPage = webpageNew(seedURL, 0);

	// test that seed url is valid url, populate html 
	if (!seedPage){
		// failure, could not fetch page html
		fprintf(stderr, "Error: Could not retrieve web page at seed url\n");
		// free data structures and exit 
		bag_delete(pageBag);
		hashtable_delete(urlTable);
		exit(4);
	}

	// seedurl is valid, insert corresponding page into bag
	bag_insert(pageBag, seedPage);
	// insert seedurl into table
	hashtable_insert(urlTable, seedURL, NULL);

	WebPage *nextPage;

	// while the bag of pages is not empty
	while ( (nextPage = bag_extract(pageBag)) != NULL) {
		
		// extract the next page and save it to directory
		pagesave(nextPage, pageDirectory);

		if (nextPage->depth < maxDepth) {
			// depth valid, scan page for more urls
			pagescan(nextPage, pageBag, urlTable);
		}
		sleep(1);
	}
	/* CLEANUP */

	exit(0);
}

/*
 * pagesave: write the information from specified page
 * to new file in pageDirectory; file is of format:
 * 		page URL
 * 		depth
 * 		page html
 *      ........
 * return true if pagesave was successful, false otherwise
 */
bool pagesave(WebPage *page, char *pageDr)
{
	// static docID maintained throughout calls to pagesave
	static int docID = 1;
	FILE *fp;
	int a, b, c;

	// allocate memory for the filename to be built
	char *filename = count_malloc_assert(strlen(pageDr) + numDigits(docID) + 2,
										 MALLOC_ERROR );
	// concatenate the directoy name with docID
	sprintf(filename, "%s/%d", pageDr, docID);

	// open file for writing, check for failure
	fp = fopen(filename, "w");
	if (!fp)
		return false;

	// print output to file in appropriate format 
	a = fprintf(fp, "%s\n", page->url);
	b = putInt(page->depth, fp);
	c = fprintf(fp, "%s\n", page->html);

	fclose(fp);
	count_free(filename);
	docID++;   // increment docID for next file

	// WE CAN MAKE THIS BETTER, DUMB USE OF BOOLEAN LOGIC********
	if (a < 0 || b < 0 || c < 0) {
		// problem printing to file
		return false;	
	} else 
		return true;
}

/*
 * pagescan: scan the given webpage for links; insert each
 * unseen, valid link into the bag for later extraction
 */
bool pagescan(WebPage *page, bag_t *pageBag, hashtable_t *urlTable)
{
	int pos = 0;                   // position in html string
	char *result = NULL;           // will hold new url
	char *base_url = page->url;    // url of page being searched

	// loop over all urls on the page
	while ((pos = GetNextURL(page->html, pos, base_url, &result)) > 0) {
      
		if(IsInternalURL(result)) {

			if(hashtable_insert(urlTable, result, NULL)) {
				// url is valid and has not been seen before

				// create a new webpage for the url, and insert into bag
				WebPage *newPage = webpageNew(result, page->depth + 1);
				bag_insert(pageBag, newPage);
			}
    	}
    	// free and NULL result for next iteration
    	free(result);
    	result = NULL;
	}
	// successful page scan
	return true;
}

/*
 * webpageNew: create a new webpage struct
 * from the given url, marked with the specified
 * depth; use GetWebPage to populate html of page
 */
WebPage *webpageNew(char *url, int depth)
{
	// allocate memory for the new webpage
	WebPage *page = count_malloc_assert(sizeof(WebPage), 
		MALLOC_ERROR);

	// allocate memory for the webpage url string
	page->url = count_malloc_assert(strlen(url)+1, MALLOC_ERROR);
	// copy the given url into webpage
	strcpy(page->url, url);

	page->depth = depth;
	// GetWebPage expects these values
	page->html = NULL;
	page->html_len = 0;

	if (!GetWebPage(page)) {
		// failed to retrieve html
	 	webpageDelete(page);
	 	page = NULL;
	}
	return page;
}

/*
 * webpageDelete: deallocate memory
 * used by the webpage structure
 */
void webpageDelete(void *webpage)
{
	//free(webpage->url);
	free(webpage);
}

/*
 * IsWritableDirectory: determine if the given 
 * directory exists and is able to be written to;
 * return true if directory is valid, false otherwise
 * Credit: David Kotz
 */
bool isWritableDirectory(char *dir)
{
	char *fn = count_malloc_assert(strlen(dir) + 10, MALLOC_ERROR);

	// build filename string
	sprintf(fn, "%s/.crawler", dir);

	if (fopen(fn, "w") == NULL) {
		count_free(fn);
		return false;
	} else {
		count_free(fn);
		return true;
	}
}

/*
 * numDigits: recursively calculate
 * the number of digits in given integer
 */
int numDigits(int number)
{
	// integer division
	if ( (number / 10 ) == 0 ) {
		return 1;
	}
	else {
		return 1 + numDigits(number/10);
	}
}

/*
 * putInt: return pointer to string 
 * representation of given integer, caller
 * responsible for deallocation
 */
int putInt(int num, FILE *fp)
{
	int errorStatus;

	char *numStr = count_malloc_assert(numDigits(num)+1, MALLOC_ERROR);
	sprintf(numStr, "%d", num);

	errorStatus = fprintf(fp, "%s\n", numStr);
	
	count_free(numStr);
	return errorStatus;
}

/*
 * htDeleteFunc: delete the data in the 
 * hashtable of urls; data never inserted
 * into table, nothing must be freed
 */
void htDeleteFunc(void *data)
{
	;
}

/*
 * log: print log of crawler processes to stdout
 * Credit: David Kotz
 */
/*inline static void log(const char *word, const int depth, const char *url)
{
  printf("%2d %*s%9s: %s\n", depth, depth, "", word, url);
} */


