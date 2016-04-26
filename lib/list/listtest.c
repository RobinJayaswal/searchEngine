/*
 * listtest.c - testing program for list module
 *
 * usage: listtest
 * 		(no arguments)
 * output:
 * 		program prints the output of various 
 * 		tests of the list module to stdout, 
 * 		as well as information regarding expected
 * 		behavior and presence of any errors
 * functions:
 * 		> delete_func1: sample delete function for use
 * 		  with a list holding statically-allocated data;
 * 		  function body left empty
 *      > delete_func2: sample delete function for use
 * 		  with a list holding dynamically-allocated data;
 *  	  function simply deallocates the memory allocated
 *        for the user input data
 * stdin: none
 * stdout: printed results of module tests
 * stderr: error messages
 *
 * Kyle Dotterrer, April 2016
 ============================================================================*/

#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include "list.h"

void delete_func1(void *data);
void delete_func2(void *data);

int main(void)
{

 	list_t *list;                // the list
 	list_t *fake_list = NULL;    // invalid list

 	char datastring[] = "test data";

 	// create new list data structure
 	list = list_new(delete_func1);
 	printf("creating new list to hold statically-allocated data\n");

 	if (list == NULL) {
 		// error creating list
 		printf("list new failed\n");
 		exit(1);
 	}

 	// testing list node insertion
 	if ( list_insert(list, "key1", datastring) )
 		printf("new node with key 'key1' inserted\n");
 	if ( list_insert(list, "key2", datastring) )
 		printf("new node with key 'key2' inserted\n");

 	// test insertion of duplicate keys
 	if ( list_insert(list, "key2", datastring) )
 		printf("new node with key 'key2' inserted\n");
 	else
 		printf("new node with key 'key2' not inserted\n");

 	// testing list find
 	printf("find %s returns %s\n", "key1", (char*) list_find(list, "key1"));
 	printf("find %s returns %s\n", "key2", (char*) list_find(list, "key2"));

 	// test search for nonexistent key
 	printf("find %s returns %s\n", "key3", (char*) list_find(list, "key3"));

 	// test list deletion
 	list_delete(list);
 	printf("list deletion successful\n\n");

 	// create new list, with different delete function
 	list = list_new(delete_func2);
 	printf("creating new list to hold statically-allocated data\n");

 	if (list == NULL) {
 		// error creating list
 		printf("list new failed\n");
 		exit(1);
 	}

 	// dynamically allocate string data
 	char *data1 = malloc(strlen("data1"));
 	char *data2 = malloc(strlen("data2"));
 	strcpy(data1, "data1");
 	strcpy(data2, "data2");

 	// insert data into list, inform user
 	if (list_insert(list, "data1", data1))
 		printf("node with key 'data1' inserted, dynamically-allocated data: 'data1'\n");
 	if (list_insert(list, "data2", data2))
 		printf("node with key 'data2' inserted, dynamically-allocated data: 'data2'\n");

 	// test find with dynamically allocated data
 	printf("find %s returns %s\n", "data1", (char*) list_find(list, "data1"));
 	printf("find %s returns %s\n", "data2", (char*) list_find(list, "data2"));

 	// and delete the entire structure
 	list_delete(list);
 	printf("list deletion successful\n\n");

 	// test insertion into non-allocated list
 	if ( !list_insert(fake_list, "key3", datastring) )
 		printf("insertion into non-allocated list fails\n");

 	exit(0);
}

/*
 * delete_func1: sample delete function used 
 * with user input data that is not dynamically
 * allocated
 */
void delete_func1(void *data) 
{
	;
}

/*
 * delete_func2: sample delete function used
 * with user input data that is dynamically
 * allocated
 */
void delete_func2(void *data)
{
	free(data);
}
