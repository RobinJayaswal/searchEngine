/*
 * bag.h - header file for bag module
 *
 * Kyle Dotterrer, April 2016
 ============================================================================*/

#ifndef __BAG_H
#define __BAG_H

/*============================= global types ================================*/

typedef struct bag bag_t;  


/*============================== functions ==================================*/

/*
 * bag_new: create a new empty bag data structure
 * must specify the function used to delete stored data
 */
bag_t* bag_new( void (*deletefunc)(void *data) );

/*
 * bag_insert: add a new item to the bag
 */
void bag_insert(bag_t *bag, void *data);

/*
 * bag_extract: remove any item from the bag and return 
 * it to the caller, return NULL if bag is empty
 */
void* bag_extract(bag_t *bag);

/*
 * bag_delete: delete the whole data structure,
 * as well as its contents
 */
void bag_delete(bag_t *bag);

/* Iterate over all items in bag (in undefined order):
 * call itemfunc for each item, passing (arg, data).
 */
void bag_iterate(bag_t *bag,
		 void (*itemfunc)(void *arg, void *data),
		 void *arg);

#endif // __BAG_H