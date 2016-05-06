/* ============================================================================
 *
 * index.h - header file for index module
 *
 * Robin Jayaswal, Kyle Dotterrer, May 2016
 *
 * ========================================================================= */

#include "../lib/hashtable/hashtable.h"

/*
 * indexSave: save the given index data structure
 * to a new filename with name indexFilename;
 * handles all memory allocations
 */
void indexSave(char *indexFile, hashtable_t *index);

/*
 * indexLoad: load the index data contained within
 * indexFile into inverted index data structure;
 * assumes memory for index data structure alloc'ed 
 * returns 0 when index loading is successful, 
 * nonzero return value otherwise
 */
int indexLoad(char *indexFile, hashtable_t *index);