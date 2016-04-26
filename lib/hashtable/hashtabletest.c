/*
 * hastabletest.c - test program for 'hastable' module
 *
 * Kyle Dotterrer, April 2016
 ============================================================================*/

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "hashtable.h"

void delete_func1(void *data);
void delete_func2(void *data);

int main(void)
{
 	hashtable_t *ht;
 	hashtable_t *fakeht = NULL;

 	// initialize the new hashtable
 	ht = hashtable_new(100, delete_func1);
 	printf("creating new hashtable to hold statically-allocated data\n");

 	if (ht == NULL) {
 		// test for initialization error
 		printf("hashtable new failed\n");
 		exit(1);
 	}

 	// data string for first set of tests
 	char datastring[] = "test data";

 	// initial insertions
 	if (hashtable_insert(ht, "data1", datastring))
 		printf("inserted 'test data' with key 'data1'\n");
 	if (hashtable_insert(ht, "data2", datastring))
 		printf("inserted 'test data' with key 'data2'\n");

 	// test insertion of existing key
 	if (hashtable_insert(ht, "data2", datastring))
 		printf("inserted 'test data' with key 'data2'\n");
 	else
 		printf("'test data' with key 'data2' not inserted\n\n");

 	// test find function
 	printf("find %s returns %s\n", "data1", (char*) hashtable_find(ht, "data1"));
 	printf("find %s returns %s\n", "data2", (char*) hashtable_find(ht, "data2"));

 	// delete the data structure
 	hashtable_delete(ht);
 	printf("hashtable deleted successfully\n\n");

 	// initialize new hashtable 
 	ht = hashtable_new(100, delete_func2);
 	printf("creating new hashtable to hold dynamically allocated data\n");

 	// allocate memory for strings
 	char *data1 = malloc(strlen("data1"));
 	char *data2 = malloc(strlen("data2"));
 	strcpy(data1, "data1");
 	strcpy(data2, "data2");

 	// perform insertions with dynamically allocated data
 	if (hashtable_insert(ht, "data1", data1))
 		printf("inserted dynamically allocated 'data1' with key 'data1'\n");
 	if (hashtable_insert(ht, "data2", data2))
 		printf("inserted dynamically allocated 'data2' with key 'data2'\n\n");

 	// test find with dynamically allocated data
 	printf("find %s returns %s\n", "data1", (char*) hashtable_find(ht, "data1"));
 	printf("find %s returns %s\n", "data2", (char*) hashtable_find(ht, "data2"));

 	hashtable_delete(ht);
 	printf("hashtable deleted successfully\n\n");

 	// test collision handling
 	ht = hashtable_new(1, delete_func1);
 	printf("creating hashtable with size 1\n");

 	if (ht == NULL)
 		printf("hashtable creation fails\n");

 	// insert elements that will hash to same slot
 	if (hashtable_insert(ht, "data1", datastring))
 		printf("inserted 'test data' with key 'data1'\n");
 	if (hashtable_insert(ht, "data2", datastring))
 		printf("inserted 'test data' with key 'data2'\n");

 	// test find with collisions
 	printf("find %s returns %s\n", "data1", (char*) hashtable_find(ht, "data1"));
 	printf("find %s returns %s\n", "data2", (char*) hashtable_find(ht, "data2"));

 	hashtable_delete(ht);
 	printf("hashtable deleted successfully\n\n");

 	// demonstrate hashtable initialization with size 0
 	ht = hashtable_new(0, delete_func1);
 	printf("creating hashtable with size 0\n");

 	if (ht == NULL)
 		printf("hashtable creation fails\n\n");

 	// demonstrate insertion failure into invalid hashtable
 	if (hashtable_insert(fakeht, "data1", datastring))
 		printf("insertion into invalid hashtable succeeds\n");
 	else
 		printf("insertion into invalid hashtable fails\n");
}

/*
 * delete_func1: sample delete function to
 * delete user supplied data that is not
 * dynamically allocated, no operations necessary
 */
void delete_func1(void *data)
{
	;
}

/*
 * delete_func2: sample delete function to
 * delete user supplied data that is dynamically
 * allocated, data must be deallocated by user
 */
void delete_func2(void *data)
{
	free(data);
}



