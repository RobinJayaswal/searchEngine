/*
 * bagtest.c - test program for bag module
 *
 * usage: bagtest
 *     (no arguments)
 * ouput:
 *     program prints the results of various tests of the 
 *     bag module to stdout, while also informing the user
 *     of what the desired output is
 * functions:
 *     > delete_func1: a sample delete function passed to the bag
 *       module which the module then uses to delete user input 
 *       data that is statically allocated
 *     > delete_func2: a sample delete function passed to the bag
 *       module which the module then uses to delete user input
 *       data that is dynamically allocated 
 * stdin: none
 * stdout: printed test results
 * stderr: error messages
 *  
 * Kyle Dotterrer, April 2016
 ============================================================================*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "bag.h"

void delete_func1(void *data);
void delete_func2(void *data);

int main(void)
{
 	bag_t *bag;              // the bag
 	bag_t *fakebag = NULL;   // invalid bag
 	void *extracted;

 	// initialize a new bag
 	bag = bag_new(delete_func1);
 	printf("creating new bag to hold statically allocated data\n");

 	if (bag == NULL) {
 		// inform user if error occurs
 		fprintf(stderr, "bag_new failed\n");
 		exit(1);
 	}

 	// insert some items into bag
 	// strings not dynamically allocated
 	bag_insert(bag, "data1");
 	bag_insert(bag, "data2");
 	bag_insert(bag, "data3");
 	bag_insert(bag, "data4");
 	bag_insert(bag, "data1");

 	// inform user of what was inserted
 	printf("insert item with 'data1' string literal\n");
 	printf("insert item with 'data2' string literal\n");
 	printf("insert item with 'data3' string literal\n");
 	printf("insert item with 'data4' string literal\n");

 	// bag accepts duplicates
 	printf("insert item with 'data1' string literal\n");

 	// perform some extractions
 	printf("extract returns %s\n", (char*) bag_extract(bag));
 	printf("extract returns %s\n", (char*) bag_extract(bag));
 	printf("extract returns %s\n", (char*) bag_extract(bag));
 	printf("extract returns %s\n", (char*) bag_extract(bag));
 	printf("extract returns %s\n", (char*) bag_extract(bag));

 	// should return null
 	printf("bag empty, extract returns %s\n", (char*) bag_extract(bag));

 	// delete entire data structure
 	bag_delete(bag);
 	printf("deletion of empty bag successful\n\n");

 	// initialize a new bag
 	bag = bag_new(delete_func2);
 	printf("creating new bag to hold dynamically allocated data\n");

 	if (bag == NULL) {
 		// inform us if error occurs
 		fprintf(stderr, "bag_new failed\n");
 		exit(1);
 	}

 	// create strings with dynamically allocated memory
 	char *data1 = malloc(strlen("data1"));
 	char *data2 = malloc(strlen("data2"));
 	char *data3 = malloc(strlen("data3"));
 	strcpy(data1, "data1");
 	strcpy(data2, "data2");
 	strcpy(data3, "data3");

 	// insert dynamically allocated strings into bag
 	bag_insert(bag, data1);
 	bag_insert(bag, data2);
 	bag_insert(bag, data3);

 	// inform user of what was inserted
 	printf("insert item with 'data1' dynamically allocated string\n");
 	printf("insert item with 'data2' dynamically allocated string\n");
 	printf("insert item with 'data3' dynamically allocated string\n");

 	// test extraction of dynamically allocated data
 	printf("extract returns %s\n", (char*) bag_extract(bag));

 	// delete entire data structure
 	bag_delete(bag);
 	printf("deletion of non-empty bag successful\n\n");

 	// test extraction from invalid bag
 	extracted = bag_extract(fakebag);
 	if (extracted == NULL)
 		printf("extraction from invalid bag returns NULL\n");
 	else
 		printf("extraction from invalid bag returns non-NULL\n");

 	exit(0);
}

/*
 * delete_func1: example of delete function that
 * handles user data that is not dynamically allocated,
 * nothing needs to be done before item is freed
 */
void delete_func1(void *data)
{
	;
}

/*
 * delete_func2: example of delete function to 
 * handle data that is dynamically allocated,
 * frees the data so that item can then be freed
 */
void delete_func2(void *data)
{
	free(data);
}


