#include <stdio.h>
#include <stdlib.h>
#include "common/word.c"

char **tokenize(char *query); 

int main(const int argc, char *argv[])
{
	char *sumString = argv[1];
	printf("%s\n", sumString);

	int sum = 0;

	char **tokens = tokenize(sumString);

	for (int i = 0; i < strtol( *(tokens -1), NULL, 10); i++){
		char *word;
		int product = 1;
		while (i < strtol( *(tokens -1), NULL, 10) && ((strcmp((word = tokens[i]), "+") != 0)) ) {
			if (strcmp(word, "*") == 0)
				i++;
			else {
				int num = strtol(word, NULL, 10);
				product *= num;
				i++;
			}
		}
		sum += product;
	}

	printf("Total was: %d\n", sum);
}


char **tokenize(char *query) 
{
 	char *word;
 	int wordCount = 0;
 	int arrLen = 10;
 	char **tokens = calloc(arrLen, sizeof(char*));

 	word = strtok(query, " ");
 	
 	while( word != NULL) {

 		if (++wordCount > arrLen) {
 			arrLen = arrLen * 2;
 			tokens = realloc(tokens, arrLen);
 		}

 		tokens[wordCount] = malloc(strlen(word) + 1);
 		strcpy(tokens[wordCount], word);
 	
 		word = strtok(NULL, " ");
 	}

 	char *arrLenStr = malloc(20 + 2);
 	sprintf(arrLenStr, "%d", wordCount);

 	tokens[0] = malloc(strlen(arrLenStr));
 	// store array length in first position
 	strcpy(tokens[0], arrLenStr);

 	// return pointer to first element of actual array
 	return ++tokens;
 }