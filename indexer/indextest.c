/* ============================================================================
 *
 * indextest.c 
 *
 * Robin Jayaswal, Kyle Dotterrer, May 2016
 *
 * ========================================================================= */

#include <stdio.h>
#include <stdlib.h>

int parseArguments(const int argc, char *argv[]);

int main(const int argc, char *argv[])
{
	int argStatus;

	argStatus = parseArguments(argc, argv);

	if (argStatus != 0)
		exit(argStatus);

	exit(0);
}

/*
 * parseArguments: validate command line arguments
 * passed to indextester; return nonzero integer if
 * arguments are invalid, 0 otherwise
 */
int parseArguments(const int argc, char *argv[])
{
	char *progName = argv[0];

	if (argc != 3){
		fprintf(stderr, "Error: invalid arguments\n");
		fprintf(stderr, "usage: %s oldIndexFilename newIndexFilename\n", progName);
		return 1;
	}

	return 0;
}

