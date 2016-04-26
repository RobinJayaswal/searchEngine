/*
 * counterstest.c - test program for counters module
 *
 * usage: counterstest
 *		(no arguments)
 * output: 
 * 		program prints the results of various
 *	 	tests of the counters module to stdout, 
 *      while also informing the user of what the
 *      desired output is and if an error occured
 * functions:
 *		no local functions defined
 * stdin: none
 * stdout: printed results of module tests
 * stderr: error messages
 *
 * Kyle Dotterrer, April 2016
 ============================================================================*/

#include <stdio.h>
#include <stdlib.h>
#include "counters.h"

 int main(void) 
 {
 	counters_t *ctrs;              // the counters
 	counters_t *fakectrs = NULL;   // invalid struct
 	int retrieved;

 	// initialize a new data structure
 	ctrs = counters_new();

 	if (ctrs == NULL) {
 		printf("counters new failed\n");
 		exit(1);
 	}

 	// add some values
 	counters_add(ctrs, 1);
 	counters_add(ctrs, 2);
 	counters_add(ctrs, 3);
 	counters_add(ctrs, 1);

 	// inform the user of what was added
 	printf("added key %d\n", 1);
 	printf("added key %d\n", 2);
 	printf("added key %d\n", 3);
 	printf("added key %d\n\n", 1);

 	// perform get operations, displaying results
 	printf("get %d returns %d\n", 1, counters_get(ctrs, 1));
 	printf("get %d returns %d\n", 2, counters_get(ctrs, 2));
 	printf("get %d returns %d\n", 3, counters_get(ctrs, 3));

 	// test get of value not inserted
 	printf("get %d returns %d\n", 4, counters_get(ctrs, 4));

 	// delete the data structure
 	counters_delete(ctrs);
 	printf("counters delete successful\n\n");

 	// test get when invoked on invalid counters 
 	retrieved = counters_get(fakectrs, 1);
 	printf("get %d returns %d when invoked on invalid counters\n", 1, retrieved);
 	retrieved = counters_get(fakectrs, 2);
 	printf("get %d returns %d when invoked on invalid counters\n", 2, retrieved);

 	exit(0);
 }

 