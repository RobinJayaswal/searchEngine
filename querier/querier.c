/* ============================================================================
 *
 * querier.c
 *
 * Robin Jayaswal, Kyle Dotterrer, May 2016
 *
 =========================================================================== */

/* To use */
/************* *******************/

#include <stdlib.h>
#include <stdio.h>
#include "../common/index.h"

 /************* Function Prototypes *************/
 const int parseArguments(const int argc, char *argv[]);
 static bool isCrawlerDirectory(char *dir)



 int main(const int argc, char *argv[])
 {
 	// parse arguments
 	char *programName = argv[0];

 	int argStatus = parseArguments(argc, argv);
 	if (argStatus != 0)
 		exit(argStatus);

 	// argument tests passed
 	char *pageDir = argv[1];
 	char *indexFN = argv[2];

 	

 	

 }


/* parseArguments: check arguments passed to querier
 * returns nonzero if incorrect number of invalid pageDirectory
 * passed. zero otherwise
 */ 
 const int parseArguments(const int argc, char *argv[])
 {
 	char *programName = argv[0];
 	if (argc != 3) {
 		fprintf(stderr, "Error: Incorrect number of arguments\n");
 		fprintf(stderr, "Usage: %s pageDirectory indexFilename\n", programName);
 		return 1;
 	}

 	char *pageDir = argv[1];
 	char *indexFN = argv[2];

 	if (!isCrawlerDirectory(pageDir)){
 		fprintf(stderr, "Error: %s is not a proper crawler directory\n", pageDir);
 		fprintf(stderr, "See querier.c README for crawler directory reqs\n");
 		return 3;
 	}

 	if ( (FILE *fp = fopen(indexFN)) == NULL){
 		fprintf(stderr, "Error: %s is not a readable file\n", indexFN);
 		fprintf(stderr, "See querier.c README.md for index filename reqs\n");
 		return 5;
 	}
 	
 	// arguments valid to go ahead
 	return 0;

 }


 /*
 * isCrawlerDirectory: return true if the
 * page directory contains the .crawler
 * file created by crawling process, false
 * otherwise
 */
static bool isCrawlerDirectory(char *dir)
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