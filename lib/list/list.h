/*
 * list.h - header file for list module
 *
 * Kyle Dotterrer, April 2016
 ============================================================================*/

#ifndef __LIST_H
#define __LIST_H

/*============================= global types ================================*/

typedef struct list list_t;


/*============================== functions ==================================*/

/*
 * list_new: create a new empty list data structure
 */ 
list_t* list_new( void (*deletefunc)(void *data) );

/*
 * list_find: return data for the given key, or
 * NULL if the key is not found
 */
void* list_find(list_t *list, char *key);

/*
 * list_insert: return true if new item with key and
 * data was inserted, return false if item with key
 * was already in list, if list is NULL, or other 
 * error is encountered
 */
bool list_insert(list_t *list, char *key, void *data);

/*
 * list_delete: delete the entire data 
 * structure, including its contents
 */
void list_delete(list_t *list);

 #endif // __LIST_H
