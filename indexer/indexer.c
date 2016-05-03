/* ============================================================================
 *
 * indexer.c 
 *
 * Robin Jayaswal, Kyle Dotterrer, May 2016
 *
 * ========================================================================= */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

int parseArguments(const int argc, char *argv[]);
bool isCrawlerDirectory(char *dir);

int main(const int argc, char* argv[])
{
	int argStatus;
	char *pageDir;
	char *indexFN;

	argStatus = parseArguments(argc, argv);

	if (argStatus != 0)
		exit(argStatus);

	pageDir = argv[1];
	indexFN = argv[2];
	
	exit(0);
}

/*
 * parseArguments:
 */
int parseArguments(const int argc, char *argv[])
{
	char *progName = argv[0];

	if (argc != 3) {
		fprintf(stderr, "Error: invalid arguments\n");
		fprintf(stderr, "usage: %s pageDirectory indexFilename\n", progName);
		return 1;
	}

	if (!isCrawlerDirectory(argv[1])) {
		fprintf(stderr, "Error: invalid page directory\n");
		return 3;
	}
	return 0;
}

/*
 * isCrawlerDirectory:
 */
bool isCrawlerDirectory(char *dir)
{
	char *fn = malloc(strlen(dir) + 10);
	FILE *fp;

	// build filename string
	sprintf(fn, "%s/.crawler", dir);

	// create file, check for opening error
	if ( (fp = fopen(fn, "r")) == NULL) {
		free(fn);
		return false;
	} else {
		free(fn);
		fclose(fp);
		return true;
	}
}
