/**
 * maxTweeter takes in a csv file name and will calculate
 * the top 10 Tweeters by name.
 * 
 * Authors: Yiping (Allison) Su and Joanne Chang
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* max characters in the name field */
#define NAME_LENGTH 20
/* max characters in one csv line */
#define MAX_LINE 1024

/**
 * Tweeters defines the data struct which
 * stores the username and the number
 * of tweets the user has made.
 */
typedef struct tweeter
{
	char *name;
	int count;
} Tweeter;

/**
 * Node defines the data structure which stores the
 * tweeter information.
 * 
 * It contains the user information and the pointers
 * to both neighbors of the node.
 */
typedef struct node
{
	Tweeter user;
	struct node *prev;
	struct node *next;
} Node;

Node *createNode(char *name);
char *extractName(char *str, int namePos);
Node *initialize(void);
void insertToList(Node *list, char *name);
int parseName(FILE *fileName);
void processData(FILE *fileName, int namePos, Node *list);

int main(int argc, char *argv[])
{
	// TODO: Implement this
	FILE *fileName = fopen(argv[1], "r");
	int namePos = parseName(fileName);
	Node *list = initialize();
	processData(fileName, namePos, list);


	// TODO: Free all memory when we reach here (at the end)
	fclose(fileName);
	return 0;
}

/**
 * initialize creates the very first node which starts as the
 * HEAD of the doubly-linked list.
 * 
 * @return the pointer to the HEAD
 */
Node *initialize(void) {
	Node *list = malloc(sizeof(Node));
	Tweeter *firstTweeter = malloc(sizeof(Tweeter));
	firstTweeter -> count = 0;
	firstTweeter -> name = NULL;
	list -> user = *firstTweeter;
	return list;
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
int parseName(FILE *fileName)
{
	// TODO: Make sure lines longer than 1024 are handled 
	int index = 0;
	char buff[MAX_LINE];
	char *str = strdup(fgets(buff, MAX_LINE, (FILE*)fileName));
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

/**
 * processData takes in a file pointer, index of the Name field, and
 * the pointer to the Empty HEAD of the list and processes the data of the
 * csv file.
 * 
 * It will use a while loop to iterate through each line and add tweet info
 * to the linked list using insertList().
 * 
 * @return void
 */
void processData(FILE *fileName, int namePos, Node *list)
{
	char buff[MAX_LINE];
	while (!feof(fileName)) {
		char *str = fgets(buff, MAX_LINE, fileName);
		char *name = extractName(str, namePos);

		insertToList(list, name);
		printf("line item: %s - %d\n", list->user.name, list->user.count);

	}
}

/**
 * extractName takes in a csv line and name position
 * and returns the string at the given index.
 * 
 * @return the supposed 'name' string at the index value
 */
char *extractName(char* str, int namePos)
{
	int index = 0;
	char *token = str, *end = str;
	while (token != NULL) {
		strsep(&end, ",");
		if (index == namePos) {
			return token;
		}
		token = end;
		index++;
	}
	return "";
}

/**
 * createNode takes in a character string, name, and creates
 * a new Node and new Tweeter value.
 * 
 * @return The pointer of the new node to be inserted in a list
 * with filler values
 */
Node *createNode(char *name)
{
	Node *newNode = malloc(sizeof(Node));
	newNode -> next = NULL;
	newNode -> prev = NULL;
	Tweeter *newTweeter = malloc(sizeof(Tweeter));
	newTweeter -> count = 1;
	newTweeter -> name = name;
	newNode -> user = *newTweeter;
	return newNode;
}

/**
 * insertToList takes in a pointer to the start of the doubly
 * linked list and a char string, name.
 * 
 * It inserts the node into the correct part of the list and
 * calls on createNode for new nodes that need to be created.
 *
 * @return void
 */
void insertToList(Node *list, char *name)
{
	// TODO: Finish def
	if ((list -> next == NULL) && (list -> prev == NULL) && 
		(list -> user.name == NULL)) {
			// The node list is empty -- we start at item one
			// HEAD of the list
			list -> user.count = 1;
			list -> user.name = name;
	}
	// List is not empty so we must do an insertion sort
}