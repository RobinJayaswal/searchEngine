/*
 * hashtable.c - hashtable module implemented as array of linked lists
 *  store arbitrary data using void*
 *
 * Kyle Dotterrer, April 2016
 ============================================================================*/

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "hashtable.h"
#include "list.h"
#include "jhash.h"

/*============================= global types ================================*/

// hashtable structure, containing table and size
struct hashtable {
	int size;                         // number of slots in table
	list_t **table;                   // pointer to pointer array
	void (*deletefunc)(void *data);   // function to delete stored data
};
typedef struct hashtable hashtable_t;


/*============================ global functions =============================*/
/* 
	see module header file (hashtable.h) 
	for information about exported functions 
*/

/*============================ local functions ==============================*/

/**************** hashtable_new *****************/
hashtable_t* hashtable_new(const int num_slots, void (*deletefunc)(void *data))
{
	hashtable_t *ht;

	if (num_slots < 1 || deletefunc == NULL)
		// invalid size for the table or invalid delete function
		return NULL;

	// allocate memory for hashtable structure
	ht = malloc(sizeof(struct hashtable));
	if (ht == NULL)
		// error allocating memory for hashtable structure
		return NULL;
	
	// allocate memory for table itself
	ht->table = calloc(num_slots, sizeof(list_t *));
	if (ht->table == NULL) {
		// error allocating memory for table
		free(ht);
		return NULL;
	}
	// set the size of the hashtable
	ht->size = num_slots;
	// set the delete function
	ht->deletefunc = deletefunc;

	return ht;
}

/*************** hashtable_find ****************/
void* hashtable_find(hashtable_t *ht, char *key)
{
	int index;
	list_t *list;
	void *data;

	if (ht == NULL)
		// nothing to do if null pointer given
		return NULL;

	// get the index of the key in the table
	index = (int) JenkinsHash(key, ht->size);
	// find the list at that index
	list = ht->table[index];

	data = list_find(list, key);
	return data;
}

/************** hashtable_insert ***************/
bool hashtable_insert(hashtable_t *ht, char *key, void *data)
{
	int index;

	if (ht == NULL)
		// nothing to do if null pointer given
		return false;

	// index where key should be inserted
	index = (int) JenkinsHash(key, ht->size);

	if (ht->table[index] == NULL) 
		// no list currently in slot, create new list
		ht->table[index] = list_new(ht->deletefunc);
	
	return list_insert(ht->table[index], key, data);
}

/************** hashtable_delete ***************/
void hashtable_delete(hashtable_t *ht)
{
	int i;

	if (ht == NULL)
		return;

	// iterate over each slot in table
	for (i = 0; i < ht->size; i++){
		// delete the list in that slot
		list_delete(ht->table[i]);
	}
	// deallocate the table itself
	free(ht->table);
	// deallocate the structure
	free(ht);
}




