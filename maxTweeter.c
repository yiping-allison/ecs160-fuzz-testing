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
 * Tweeter defines the data struct which
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

/**
 * Link defines an object which contains the address
 * to the HEAD and TAIL of the double-linked list
 */
typedef struct doublelink
{
	struct node *head;
	struct node *last;
} Link;

char *allocateName(char *nameToCopy);
Node *createNode(char *name, int initial);
char *extractName(char *str, int namePos);
int findUser(char *name, Link *info);
int getNameIndex(FILE *fileName);
void insertAtLast(char *name, Link *info);
void insertToList(Node *list, char *name, Link *info);
void processData(FILE *fileName, int namePos, Node *list, Link *info);
void swap(Node *left, Node *right, Link *info);

void printList(Link *info);

int main(int argc, char *argv[])
{
	// TODO: Implement this
	FILE *fileName = fopen(argv[1], "r");
	int namePos = getNameIndex(fileName);
	Link *info = malloc(sizeof(Link));
	Node *first = createNode(NULL, 1);
	info -> head = first;
	info -> last = first;
	processData(fileName, namePos, first, info);

	// printf("In the main func -- final answer\n");
	// printList(info);
	// TODO: Free all memory when we reach here (at the end)
	fclose(fileName);
	return 0;
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
int getNameIndex(FILE *fileName)
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
void processData(FILE *fileName, int namePos, Node *list, Link *info)
{
	char buff[MAX_LINE];
	while (!feof(fileName)) {
		char *str = fgets(buff, MAX_LINE, fileName);
		if (!str) {
			return;
		}
		char *name = extractName(str, namePos);
		printf("name: %s\n", name);
		insertToList(list, name, info);
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
	return NULL;
 }

/**
 * createNode takes in a character string and and integer and creates
 * a new Node and new Tweeter value. If the integer equals, 1, it means
 * that we're dealing with the first INITIAL node. Otherwise it's a regular node.
 * 
 * @return The pointer of the new node to be inserted in a list
 *         with filler values
 */
Node *createNode(char *name, int initial)
{
	Node *newNode = malloc(sizeof(Node));
	newNode -> next = NULL;
	newNode -> prev = NULL;
	Tweeter *newTweeter = malloc(sizeof(Tweeter));
	if (initial == 1) {
		// initial HEAD node
		newTweeter -> count = 0;
		newTweeter -> name = NULL;
	} else {
		newTweeter -> count = 1;
		newTweeter -> name = name;
	}
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
void insertToList(Node *list, char *name, Link *info)
{
	// TODO: Finish def -- what I'm working on
	if (!(info -> head -> user.name)) {
		// The node list is empty -- we start at item one
		// HEAD of the list
		info -> head -> user.count = 1;
		info -> head -> user.name = allocateName(name);
		return;
	}
	// There're items in the list -- find the user first
	int res = findUser(name, info);
	if (res == -1) {
		insertAtLast(name, info);
	}
}

/**
 * findUser takes in a name string and the data struct to the head and
 * tail of the linked list and finds if the username can be found
 * in our current dataset. 
 * 
 * If we find a user, we add the count in our dataset and sort it to its correct
 * place and return 1. Otherwise, we return -1 -- not found.
 * 
 * @return 1 or -1 which represents found and not found
 */
int findUser(char *name, Link *info)
{
	// TODO: Finish this
	Node *current = info -> head;
	while (strcmp(current -> user.name, name) != 0) {
		if (current -> next == NULL) {
			return -1;
		} else {
			current = current -> next;
		}
	}	
	// if we reach here, we found the user
	++(current -> user.count);
	if ((current -> prev) == NULL) {
		// Nothing on the left -- current should be the new HEAD
		info -> head = current;
		return 1;
	}
	while ((current -> user.count) > (current -> prev -> user.count)) {
		// TODO: Swap current with current -> prev
		swap(current -> prev, current, info);
		if ((current -> prev) == NULL) {
			// we reached the end
			break;
		}
	}
	return 1;
}

void swap(Node *left, Node *right, Link *info)
{
	// FIXME: Errors in swap relating to names > 2
	// case only 2 nodes
	if ((left -> prev == NULL) && (right -> next == NULL)) {
		right -> prev = NULL;
		right -> next = left;
		left -> next = NULL;
		left -> prev = right;
		info -> head = right;
		info -> last = left;
		return;
	}
	// TODO: swapFirst
	if ((left -> prev == NULL) && (right -> next != NULL)) {
		// We do a first swap
	}
	// TODO: swapEnd
	if ((right -> next == NULL) && (left -> prev != NULL)) {
		// End node swap
		right -> prev = left -> prev;
		right -> next = left;
		left -> prev = right;
		left -> next = NULL;
		info -> last = left;
		return;
	}
	// TODO: normal (adjacent) swap
	
}

/**
 * insertAtLast takes in a name string and the data struct
 * which stores the pointers to the head and tail of the linked list
 * and adds an element to the very end of the linked list.
 * 
 * @param name
 * @param info
 * @return void
 */
void insertAtLast(char *name, Link *info)
{
	Node *newNode = createNode(name, 0);
	newNode -> user.name = allocateName(name);
	info -> last -> next = newNode;
	newNode -> prev = info -> last;
	info -> last = newNode;
}

/**
 * allocateName is a utility function which takes in an address
 * pointing to a name char array and copies that name into a new memory
 * location (used for new username creation)
 * 
 * @return the new address to the name
 */
char *allocateName(char *nameToCopy)
{
	char *newName = malloc(sizeof(nameToCopy));
	strcpy(newName, nameToCopy);
	return newName;	
}

/**
 * printList takes in the data struct which stores the head and the
 * tail of the links and prints the whole linked list
 * created so far.
 * 
 * This is used for debugging purposes
 */
void printList(Link *info)
{
	Node *current = info -> head;
	printf("\n");
	printf("Printing list:\n");
	while (current != NULL) {
		printf("user name: %s\n", current ->user.name);
		printf("user count: %d\n", current ->user.count);
		current = current -> next;
	}
}
