/*
 * list.c - list module implemented as linked list
 *  store arbitrary data using void*
 *
 * Kyle Dotterrer, April 2016
 ============================================================================*/

#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include "list.h"

/*=============================== local types ===============================*/

// individual node in linked list
struct listnode {
	char *key;               // node key is string
	void *data;              // node data is pointer to any type
	struct listnode *next;   // pointer to subsequent node
};
typedef struct listnode listnode_t;


/*============================== global types ===============================*/

// singly linked list data structure
struct list {
	struct listnode *head;             // first element in list
	struct listnode *tail;             // last element in list
	void (*deletefunc)(void *data);    // function to delete list data
};
typedef struct list list_t;


/*============================ global functions =============================*/
/* 
	see module header file (list.h) 
	for information about exported functions 
*/

/*============================= local functions =============================*/
listnode_t* listnode_new(char *key, void *data);
bool key_in_list(list_t *list, char *key);
bool is_empty(list_t *list);

/****************** list_new *******************/
list_t* list_new( void (*deletefunc)(void *data) )
{
	if (deletefunc == NULL)
		// invalid delete function
		return NULL;

	// allocate memory for new list
	list_t *list = malloc(sizeof(struct list));

	if (list == NULL)
		// memory allocation error
		return NULL;

	// empty list
	list->head = NULL;
	list->tail = NULL;
	list->deletefunc = deletefunc;
	return list;
}

/*
 * listnode_new: create and return new listnode with
 * given key and data, return NULL if errors 
 */
listnode_t* listnode_new(char *key, void *data)
{
	// allocate memory for the new node
	listnode_t *node = malloc(sizeof(struct listnode));

	if (node == NULL)
		// memory allocation error
		return NULL;

	// allocate memory for the key
	node->key = malloc(strlen(key) + 1);

	if (node->key == NULL) {
		// error allocating memory for key
		free(node);
		return NULL;
	}else {
		// copy key into memory address
		strcpy(node->key, key);
		// set data and next pointers
		node->data = data;
		node->next = NULL;
		return node;
	}
}

/****************** list_find *******************/
void* list_find(list_t *list, char *key)
{
	listnode_t *current;

	if (list == NULL || is_empty(list))
		// nothing to do if null pointer, or empty 
		return NULL;

	// current initialized as first item in list
	current = list->head;

	// stop traversing after tail is checked
	// tail->next is NULL reference
	while (current != NULL) {
		if (strcmp(current->key, key) == 0)
			// keys match
			return current->data;
		// move to the next node
		current = current->next;
	}
	// key was not found in the list
	return NULL;
}

/***************** list_insert ******************/
bool list_insert(list_t *list, char *key, void *data)
{
	listnode_t *inserted;

	if (list == NULL)
		return false;

	inserted = listnode_new(key, data);
	if (inserted == NULL) 
		// error creating new node
		return false;

	if (is_empty(list)) {
		// insert into empty list
		list->head = inserted;
		list->tail = inserted;
		return true;
	}
	// list not empty
	if (key_in_list(list, key))
		// do not perform insertion if key present
		return false;
	else {
		// insert at tail of nonempty list
		list->tail->next = inserted;
		list->tail = inserted;
		return true;
	}
}

/***************** list_delete ******************/
void list_delete(list_t *list)
{
	listnode_t *current;

	if (list == NULL)
		return;

	current = list->head;

	// traverse list until no nodes remain
	while (current != NULL) {
		// splice current out of list
		list->head = current->next;

		// delete current node's data
		(*list->deletefunc)(current->data);
		// deallocate memory for current node's key
		free(current->key);
		// deallocate memory for the current node
		free(current);

		// update pointer to consider next node
		current = list->head;
	}
	// deallocate the entire structure
	free(list);
}

/*
 * key_in_list: return true if the specified
 * key is already in the list, false otherwise
 * assumes list is nonempty
 */
bool key_in_list(list_t *list, char *key)
{
	// start at head of list
	listnode_t *current = list->head;

	// traverse the list by nodes
	while (current != NULL) {
		if (strcmp(current->key, key) == 0)
			// keys match
			return true;
		current = current->next;
	}
	// key not found
	return false;
}

/*
 * is_empty: return true if the given list
 * structure is empty, false otherwise
 */
bool is_empty(list_t *list)
{
	// list empty only when both NULL pointers
	return list->head == NULL && list->tail == NULL;
}