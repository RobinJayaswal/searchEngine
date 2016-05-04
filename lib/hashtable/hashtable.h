/*
 * hashtable.h - header file for hashtable module
 *
 * Kyle Dotterrer, April 2016
 ============================================================================*/

#include <stdbool.h>

#ifndef __HASHTABLE_H
#define __HASHTABLE_H

/*============================= global types ================================*/

typedef struct hashtable hashtable_t;


/*============================== functions ==================================*/

/*
 * hashtable_new: create a new empty hashtable 
 */
hashtable_t* hashtable_new(const int num_slots, void (*deletefunc)(void *data));

/*
 * hashtable_find: return data for given key, or
 * return NULL if key not found
 */
void* hashtable_find(hashtable_t *ht, char *key);

/*
 * hashtable_insert: return true if new item is inserted,
 * false if key already exists, the given hashtable is 
 * NULL pointer, or other error is encountered
 */
bool hashtable_insert(hashtable_t *ht, char *key, void *data);

/*
 * hashtable_delete: delete the entire data
 * structure, as well as its contents
 */
void hashtable_delete(hashtable_t *ht);

/* Iterate over all items in hashtable (in undefined order):
 * call itemfunc for each item, with (arg, key, data).
 */
void hashtable_iterate(hashtable_t *ht, 
		       void (*itemfunc)(void *arg, char *key, void *data),
		       void *arg);

#endif // __HASHTABLE_H

