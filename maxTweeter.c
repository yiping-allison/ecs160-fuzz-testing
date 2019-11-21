/**
 * maxTweeter takes in a csv file name and will calculate
 * the top 10 Tweeters by name.
 * 
 * Authors: Yiping (Allison) Su and Joanne Chang
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/**
 * Tweeter is defined by a username and number
 * of tweets they've made
 */
typedef struct Tweeter
{
	char name[20];
	int count;
} Tweeter;

int main(int argc, char *argv[]) {
	// TODO: Implement this
	FILE *fileName = fopen(argv[1], "r");
	char buff[1024];

	fgets(buff, 1024, (FILE*)fileName);
	printf("line is: %s\n", buff);

	fclose(fileName);
}