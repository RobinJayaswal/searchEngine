/*
 * bag.c - simple bag storage module
 *  store arbitrary data using void*
 *
 * Kyle Dotterrer, April 2016
 ============================================================================*/

#include <stdio.h>
#include <stdlib.h>
#include "bag.h"

/*============================== local types ================================*/
 
 // an individual item in the bag
 struct bagitem {
 	void *data;              // data held in bagitem
 	struct bagitem *next;    // pointer to next bagitem
 };
 typedef struct bagitem bagitem_t;


/*============================= global types ================================*/

// the larger structure, containing 0 or more bagitems
struct bag {
	struct bagitem *head;              // head of the bag
	void (*deletefunc)(void *data);    // function to delete bag data
};
typedef struct bag bag_t;

/*============================ global functions =============================*/
/* 
	see module header file (bag.h) 
	for information about exported functions 
*/

/*============================ local functions ==============================*/
static bagitem_t* bagitem_new(void *data);

 /****************** bag_new ********************/
bag_t* bag_new( void (*deletefunc)(void *data) ) 
{
	if (deletefunc == NULL)
		// invalid delete function
		return NULL;

	bag_t* bag = malloc(sizeof(struct bag));

	if (bag == NULL)
		// error allocating memory
		return NULL;

	// set empty bag parameters
	bag->head = NULL;
	bag->deletefunc = deletefunc;

	return bag;
}

/*
 * bagitem_new: create new bagitem and return
 * a pointer to the newly created item
 */
static bagitem_t* bagitem_new(void *data)
{
	// allocate space for the new item
	bagitem_t *item = malloc(sizeof(struct bagitem));

	if (item == NULL)
		// failure if null pointer
		return NULL;
	else {
		// otherwise, set the data
		item->data = data;
		item->next = NULL;
		return item;
	}
}

 /***************** bag_insert ******************/
void bag_insert(bag_t *bag, void *data) 
{
	bagitem_t *inserted;   // new item

	if (bag == NULL)
		// nothing to do if NULL pointer
		return;

	// create the new bag item
	inserted = bagitem_new(data);

	if (bag->head == NULL)
		// bag is empty
		bag->head = inserted;
	else {
		// insert into non-empty bag
		inserted->next = bag->head;
		bag->head = inserted;
	}
}

/***************** bag_extract ******************/
void* bag_extract(bag_t *bag)
{
	bagitem_t *extracted;   // the item being removed
	void *data;             // the data to return

	if (bag == NULL || bag->head == NULL)
		// bag is null pointer, or bag is empty
		return NULL;
	else {
		// extract leading item
		extracted = bag->head;
		// increment head to next item
		bag->head = bag->head->next;
		// hold onto the data
		data = extracted->data;
		// delete the extracted item
		free(extracted);
	}	
	return data;
}

/***************** bag_delete ******************/
void bag_delete(bag_t *bag)
{
	bagitem_t *current = bag->head;

	while (current != NULL) {
		// splice current out of bag
		bag->head = current->next;

		// delete the current item's data
		(*bag->deletefunc)(current->data);
		// free the current item
		free(current);

		// update current to next item
		current = bag->head;
	}
	// delete entire data structure
	free(bag);
}






