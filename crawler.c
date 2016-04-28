/* ========================================================================= */
/* 
 * crawler.c - crawl internal webpages, saving to specified page directory
 * 
 *
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

char* MALLOC_ERROR = "Error: memory allocation error";

bool pagesave(WebPage *page, char *pageDr);
bool pagescan(WebPage *page, bag_t *pageBag, hashtable_t *urlTable);
WebPage *webpageNew(char *url, int depth);
void webpageDelete(void *webpage);
bool isWritableDirectory(char *dir);
int numDigits(int number);

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

	/* Argument Tests Passed. Initialize Bag */

	bag_t *pageBag = bag_new(webpageDelete);
	// hashtable_t *urlTable = hashtable_new(100, free);
	hashtable_t *urlTable = NULL;

	/* Create WebPage struct for initial seed url page */

	WebPage *seedPage;
	seedPage = webpageNew(seedURL, 0);

	/* Test that seed url is valid url, and if so populate html */

	if (!seedPage){
		fprintf(stderr, "Error: Could not retrieve web page at seed url\n");
		bag_delete(pageBag);
		hashtable_delete(urlTable);
		exit(4);
	}

	bag_insert(pageBag, seedPage);
	hashtable_insert(urlTable, seedURL, NULL);

	WebPage *nextPage = bag_extract(pageBag);

	while (nextPage != NULL) {
		pagesave(nextPage, pageDirectory);

		if (nextPage->depth < maxDepth) {
			
			pagescan(nextPage, pageBag, urlTable);
		}

		
		nextPage = bag_extract(pageBag);
		sleep(1);
	}


	exit(0);

}


bool pagesave(WebPage *page, char *pageDr)
{
	static int docID = 1;

	char *filename = count_malloc_assert(strlen(pageDr) + numDigits(docID) + 2,
		MALLOC_ERROR );
	sprintf(filename, "%s/%d", pageDr, docID);

	FILE *fp;
	fp = fopen(filename, "w");
	printf("%s\n", filename);
	

	if (!fp)
		return false;

	
	/* Print output to file in appropriate format. MAKE OWN FUNCTION */

	int a, b, c;

	a = fprintf(fp, "%s\n", page->url);

	// create string representation of page depth
	char *depth = count_malloc_assert(numDigits(page->depth)+1, MALLOC_ERROR);
	sprintf(depth, "%d", page->depth);

	b = fprintf(fp, "%s\n", depth);


	c = fprintf(fp, "%s\n", page->html);

	if (a < 0 || b < 0 || c < 0)
		return false;	// problem printing to file

	fclose(fp);
	count_free(depth);
	count_free(filename);

	docID++;

	puts("finished pagesave\n");
	return true;
}

bool pagescan(WebPage *page, bag_t *pageBag, hashtable_t *urlTable)
{
	int pos = 0;
	char *result = NULL;
	char *base_url = page->url;

	while ((pos = GetNextURL(page->html, pos, base_url, &result)) > 0) {
      
		if(IsInternalURL(result)) {

			//if(hashtable_insert(urlTable, result, NULL)) {
				printf("found url: %s\n", result);

				WebPage *newPage = webpageNew(result, page->depth + 1);
				bag_insert(pageBag, newPage);
			//}
    	}
    	free(result);
    	result = NULL;
	}
	return true;
}

/*
 *
 */
int numDigits(int number)
{
	if ( (number / 10 ) == 0 ) {
		return 1;
	}
	else {
		return 1 + numDigits(number/10);
	}
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

WebPage *webpageNew(char *url, int depth)
{
	WebPage *page = count_malloc_assert(sizeof(WebPage), 
		MALLOC_ERROR);

	page->url = count_malloc_assert(strlen(url)+1, MALLOC_ERROR);
	strcpy(page->url, url);

	page->depth = depth;
	page->html = NULL;
	page->html_len = 0;

	printf("getting page with url: %s\n", page->url);

	if (!GetWebPage(page)) {
	 	webpageDelete(page);
	 	page = NULL;
	}
	//printf("got page with url: %s\n", page->url);
	return page;
}

void webpageDelete(void *webpage)
{
	/* Free Stuff */
}


