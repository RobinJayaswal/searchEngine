/*
 * counter.c - counters module implemented as linked list
 *  store ints and corresponding insertion frequency
 * 
 * Kyle Dotterrer, April 2016
 ============================================================================*/

#include <stdio.h>
#include <stdlib.h>
#include "counters.h"

/*============================== local types ================================*/
 
// single counter for individual key
struct counter {
	int key;                 // integer key 
	int value;               // number of occurences
	struct counter *next;    // pointer to next counter
};
typedef struct counter counter_t;


/*============================= global types ================================*/

// larger structure, composed of 0 or more counters
struct counters {
	// most recently added counter
	struct counter *head;
};
typedef struct counters counters_t;


/*============================ global functions =============================*/
/* 
	see module header file (counters.h) 
	for information about exported functions 
*/

/*============================ local functions ==============================*/
 counter_t* counter_new(int key);

/**************** counters_new *****************/
counters_t* counters_new(void)
 {
 	// allocate memory for counters
	counters_t *ctrs = malloc(sizeof(struct counters));
	// empty structure, NULL head
	ctrs->head = NULL;
 	return ctrs;
}

/*
 * counter_new: create new individual counter for
 * key with a starting value of 1
 */
counter_t* counter_new(int key)
{
	// allocate memory for new counter
	counter_t *ctr = malloc(sizeof(struct counter));

	if (ctr == NULL)
		// allocation failed
 		return NULL;
	else {
		ctr->key = key;
 		ctr->value = 1;
 		ctr->next = NULL;
 		return ctr;
	}
}

/**************** counters_add *****************/
void counters_add(counters_t *ctrs, int key)
{
	counter_t* added;     // new counter to be added
	counter_t *current;   // the current head of counters

	current = ctrs->head;

	if (ctrs == NULL)
		// nothing to do if null pointer
		return;

 	while (current != NULL) {
 		// traverse counters using next pointers
 		if (current->key == key) {
 			// found the key in counters, increment value
 			current->value += 1;
 			return;
 		}
 		// follow pointer to next counter
 		current = current->next;
 	}
 	// key was not found in counters, add new with key, value 1
 	added = counter_new(key);

 	if (ctrs->head != NULL)
 		// counters not empty, splice in added
 		added->next = ctrs->head;
 	// update the head of counters
 	ctrs->head = added;
}


/**************** counters_get *****************/
int counters_get(counters_t *ctrs, int key)
{
	counter_t *current;

	if (ctrs == NULL)
		// invalid counters pointer
		return 0;

	current = ctrs->head;

	while (current != NULL) {
		// traverse the counters using pointers
		if (current->key == key) {
			// found the key, return its value
			return current->value;
		}
		// follow pointer to next counter
		current = current->next;
	}
	// key was not found
	return 0;
}

/*************** counters_delete ****************/
void counters_delete(counters_t *ctrs)
{
	// current pointer starts at counters head
	counter_t *current = ctrs->head;

	// traverse the counters
	while (current != NULL) {
		// splice current out of sequence
		ctrs->head = current->next;

		// free the counter
		free(current);       

		// update the current pointer, repeat
		current = ctrs->head;
	}
	// free the entire data structure
	free(ctrs);
}



