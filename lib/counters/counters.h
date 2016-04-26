/*
 * counter.h - header file for counter module
 *
 * Kyle Dotterrer, April 2016
 ============================================================================*/ 

#ifndef __COUNTERS_H
#define __COUNTERS_H

/*============================= global types ================================*/

typedef struct counters counters_t;   // opaque to users of module


/*============================== functions ==================================*/

/*
 * counters_new: create a new empty data structure
 */
counters_t *counters_new(void);

/*
 * counters_add: if key exists, increment its counter;
 * otherwise add key and set its counter to 1
 */
void counters_add(counters_t *ctrs, int key);

/*
 * counters_get: return the current value of the 
 * counter for key, or 0 if key not found
 */
int counters_get(counters_t *ctrs, int key);

/*
 * counters_delete: delete the whole data structure,
 * including all of its contents
 */
void counters_delete(counters_t *ctrs);

#endif // __COUNTERS_H
