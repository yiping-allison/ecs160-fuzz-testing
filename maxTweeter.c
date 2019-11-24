/**
 * @file maxTweeter.c
 * @brief Prints top 10 Tweeters from CSV file
 * 
 * @author Yiping (Allison) Su
 * @author Joanne Chang
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
void freeLinkedMemory(Node *head);
int getNameIndex(FILE *fileName);
void insertAtLast(char *name, Link *info);
void insertToList(char *name, Link *info);
void printList(Node *head, int count);
void processData(FILE *fileName, int namePos, Link *info);
void swap(Node *left, Node *right, Link *info);

int main(int argc, char *argv[])
{
	FILE *fileName = fopen(argv[1], "r");
	int namePos = getNameIndex(fileName);
	Link *info = malloc(sizeof(Link));
	Node *first = createNode(NULL, 1);
	info -> head = first;
	info -> last = first;
	processData(fileName, namePos, info);
	printList(info -> head, 10);
	fclose(fileName);
	freeLinkedMemory(info -> head);
	free(fileName);
	return 0;
}

/**
 * @brief Extracts index of NAME field from CSV
 * 
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
 * @return Index of the username column
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
 * @brief Processes data from given CSV file
 * 
 * processData takes in a file pointer, index of the Name field, and
 * the pointer to the Empty HEAD of the list and processes the data of the
 * csv file.
 * 
 * It will use a while loop to iterate through each line and add tweet info
 * to the linked list using insertList().
 * 
 * @param fileName Address of file location
 * @param namePos Index of NAME value in CSV line 
 * @param info Data struct which contains address of HEAD and TAIL of list
 * @return void
 */
void processData(FILE *fileName, int namePos, Link *info)
{
	char buff[MAX_LINE];
	while (!feof(fileName)) {
		char *str = fgets(buff, MAX_LINE, fileName);
		if (!str) {
			return;
		}
		char *name = extractName(str, namePos);
		insertToList(name, info);
	}
}

/**
 * @brief Extracts the name from CSV line given an index
 *
 * @param str Address to CSV line
 * @param namePos Index of NAME value in CSV line 
 * @return The supposed 'name' string at the index value
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
 * @brief Creates a new node
 * 
 * createNode takes in a character string and and integer and creates
 * a new Node and new Tweeter value. If the integer equals, 1, it means
 * that we're dealing with the first INITIAL node. Otherwise it's a regular node.
 * 
 * @param name Address location of NAME to be used
 * @param initial Integer indicating first node or not
 * @return The pointer to the new node to be inserted in a list
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
 * @brief Handles inserting new nodes into the list
 * 
 * insertToList takes in a pointer to the start of the doubly
 * linked list and a char string, name.
 * 
 * It inserts the node into the correct part of the list and
 * calls on createNode for new nodes that need to be created.
 *
 * @param name Address location of NAME to be used
 * @param info Data struct which contains address of HEAD and TAIL of list
 * @return void
 */
void insertToList(char *name, Link *info)
{
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
 * @brief Finds a user in the doubly-linked list
 * 
 * findUser takes in a name string and the data struct to the head and
 * tail of the linked list and finds if the username can be found
 * in our current dataset. 
 * 
 * If we find a user, we add the count in our dataset and sort it to its correct
 * place and return 1. Otherwise, we return -1 -- not found.
 * 
 * @param name Address location of NAME to be used
 * @param info Data struct which contains address of HEAD and TAIL of list
 * @return 1 or -1 which represents found and not found
 */
int findUser(char *name, Link *info)
{
	Node *current = info -> head;
	while (strcmp(current -> user.name, name) != 0) {
		if (current -> next == NULL) {
			return -1;
		} else {
			current = current -> next;
		}
	}	
	// If we reach here, we found the user
	++(current -> user.count);
	if ((current -> prev) == NULL) {
		// Nothing on the left -- current should be the new HEAD
		info -> head = current;
		return 1;
	}
	while ((current -> user.count) > (current -> prev -> user.count)) {
		swap(current -> prev, current, info);
		if ((current -> prev) == NULL) {
			// We reached the end
			break;
		}
	}
	return 1;
}

/**
 * @brief Swap two nodes in the doubly-linked list
 * 
 * @param left Address of left node to be swapped
 * @param right Address of right node to be swapped
 * @param info Data struct which contains address of HEAD and TAIL of list
 * @return void
 */
void swap(Node *left, Node *right, Link *info)
{
	if ((left -> prev == NULL) && (right -> next == NULL)) {
		// There's only 2 nodes
		right -> prev = NULL;
		right -> next = left;
		left -> next = NULL;
		left -> prev = right;
		info -> head = right;
		info -> last = left;
	} else if ((left -> prev == NULL) && (right -> next != NULL)) {
		// We do a first swap
		right -> next -> prev = left;
		left -> prev = right;
		left -> next = right -> next;
		right -> next = left;
		right -> prev = NULL;
		info -> head = right;
	} else if ((right -> next == NULL) && (left -> prev != NULL)) {
		// We do a last swap
		left -> prev -> next = right;
		right -> prev = left -> prev;
		right -> next = left;
		left -> prev = right;
		left -> next = NULL;
		info -> last = left;
	} else {
		// Regular swap
		right -> next -> prev = left;
		left -> prev -> next = right;
		right -> prev = left -> prev;
		left -> next = right -> next;
		right -> next = left;
		left -> prev = right;
	}
}

/**
 * @brief Inserts node at end of list
 * 
 * insertAtLast takes in a name string and the data struct
 * which stores the pointers to the head and tail of the linked list
 * and adds an element to the very end of the linked list.
 * 
 * @param name Address location of NAME to be used
 * @param info Data struct which contains address of HEAD and TAIL of list
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
 * @brief Allocate memory space for name
 * 
 * allocateName is a utility function which takes in an address
 * pointing to a name char array and copies that name into a new memory
 * location (used for new username creation)
 * 
 * @param nameToCopy The address of string to be copied
 * @return The new address to the name
 */
char *allocateName(char *nameToCopy)
{
	char *newName = malloc(sizeof(nameToCopy));
	strcpy(newName, nameToCopy);
	return newName;	
}

/**
 * @brief Prints nodes up to specified integer
 * 
 * printList takes in the pointer to the head and prints the linked list.
 * It will print the whole linked list when count == -1, else num count.
 * 
 * @param head The memory location to first node of list
 * @param count Either the num nodes you want printed or -1 for all
 * @return void 
 */
void printList(Node *head, int count)
{
	if (count == -1) {
		printf("\nPrinting all elements:\n");
		while (head != NULL) {
			printf("%s: %d\n", head -> user.name, head -> user.count);
			head = head -> next;
		}
		return;
	}
	printf("\nPrinting Top 10:\n");
	for (int i = 0; i < count; i++) {
		if (head == NULL) {
			break;
		}
		printf("%s: %d\n", head -> user.name, head -> user.count);
		head = head -> next;
	}
}

/**
 * @brief Frees all the allocated memory in the Node List
 * 
 * @param head The memory location to first node of list
 * @return void
 */
void freeLinkedMemory(Node *head)
{
	Node *tmp;
	while (head != NULL) {
		tmp = head;
		head = head -> next;
		free(tmp -> user.name);
		free(tmp);
	}
}
