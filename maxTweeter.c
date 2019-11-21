/**
 * maxTweeter takes in a csv file name and will calculate
 * the top 10 Tweeters by name.
 * 
 * Authors: Yiping (Allison) Su and Joanne Chang
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define NAME_LENGTH 20

/**
 * Tweeter is defined by a username and number
 * of tweets they've made
 */
typedef struct Tweeter
{
	char name[NAME_LENGTH];
	int count;
} tweeter;

int parseName(FILE *fileName);

int main(int argc, char *argv[]) {
	// TODO: Implement this
	FILE *fileName = fopen(argv[1], "r");

	int namePos = parseName(fileName);
	printf("Position of name col: %d\n", namePos);

	fclose(fileName);
}

/**
 * parseName takes in a file pointer and returns
 * the index of the name position from the csv file
 *
 * parseName uses strdup() and strsep() to parse lines.
 * Unlike strtok, strsep will keep empty strings.
 * Documentation of strdup and strsep can be found at the following links:
 * 
 * https://en.cppreference.com/w/c/experimental/dynamic/strdup
 * 
 * https://www.gnu.org/software/libc/manual/html_node/Finding-Tokens-in-a-String.html
 *  
 * @param fileName The address to where the file is located
 * @return index of the username column
 */
int parseName(FILE *fileName) {
	// TODO: Make sure lines longer than 1024 are handled 
	int index = 0;
	char buff[1024];
	char *str = strdup(fgets(buff, 1024, (FILE*)fileName));
	char *token = str, *end = str;
	while (token != NULL) {
		strsep(&end, ",");
		if (strcmp(token, "name") == 0) {
			return index;
		}
		token = end;
		index++;
	}
	free(str);
	return index;
}
