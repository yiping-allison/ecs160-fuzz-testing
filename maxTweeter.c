/**
 * @file maxTweeter.c
 * @brief Prints top 10 Tweeters from CSV file
 * 
 * @author Yiping (Allison) Su
 * @author Joanne Chang
 * 
 * @bug Memory-Leak in Forced Exits From ProcessData and On
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* max characters in one csv line */
#define MAX_CHAR 1024

/* max number of lines in the csv file */
#define MAX_LINE 20000

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
 * to the HEAD and TAIL of the doubly-linked list
 */
typedef struct doublelink
{
	struct node *head;
	struct node *last;
} Link;

char *allocateName(char *nameToCopy, Link *info);
void argumentCheck(int numArg);
void checkFile(FILE *fileName);
void checkQuotes(char *name, FILE *filename, Link *info);
int commaCounter(char *line);
Node *createNode(int initial, Link *info);
char *extractName(char *str, int namePos, int *quoted, FILE *filename, Link *info);
int findUser(char *name, Link *info);
void forceExit(char *exitMsg);
void freeLinkedMemory(Node *head, Link *info);
int getNameIndex(FILE *fileName, int *quoted, int *comma);
void insertAtLast(char *name, Link *info);
void insertToList(char *name, Link *info);
void printList(Node *head, int count);
void processData(FILE *fileName, int namePos, Link *info, int *quoted, int *comma);
void removeChar(char *str, int index);
void stripQuotes(char *name, FILE *filename, Link *info);
void swap(Node *left, Node *right, Link *info);

int main(int argc, char *argv[])
{
	argumentCheck(argc);
	FILE *fileName = fopen(argv[1], "r");
	if (fileName == NULL) forceExit("\nError: No file\n");
	checkFile(fileName);
	int quoted = -1;
	int comma = 0;
	int namePos = getNameIndex(fileName, &quoted, &comma);
	Link *info = malloc(sizeof(Link));
	if (info == NULL) {
		fclose(fileName);
		forceExit("\nError: Couldn't allocate memory\n");
	}
	Node *first = createNode(1, info);
	info -> head = first;
	info -> last = first;
	processData(fileName, namePos, info, &quoted, &comma);
	printList(info -> head, 10);
	fclose(fileName);
	freeLinkedMemory(info -> head, info);
	return EXIT_SUCCESS;
}

/**
 * @brief Prints an error message and exits program
 * 
 * @param exitMsg Message relating to error
 * @return void
 */
void forceExit(char *exitMsg)
{
	printf("%s\n", exitMsg);
	exit(EXIT_FAILURE);
}

/**
 * @brief Checks the number of arguments given
 * 
 * argumentCheck will exit the program if there's an invalid program call (missing csv path), or
 * will print a message notifying only the first file will be used if multiple args are given.
 * 
 * @param numArg The number of args given
 * @return void
 */
void argumentCheck(int numArg)
{
	if (numArg < 2) {
		forceExit("\nInvalid Program Call -- Usage: ./maxTweeter.exe locationOfCSV\n");
	} else if (numArg > 2) {
		printf("\nMore than one file given -- Only the first file will be run\n");
	}
}

/**
 * @brief Checks the file size given to program
 * 
 * @param fileName The address to where the file is located
 * @return void
 */
void checkFile(FILE *fileName)
{
	fseek(fileName, 0, SEEK_END);
	long fileSize = 0;
	fileSize = ftell(fileName);
	if (fileSize == 0) {
		fclose(fileName);
		forceExit("\nError: Nothing in CSV file\n");
	} else if (fileSize > (sizeof(char) * (MAX_CHAR * MAX_LINE))) {
		fclose(fileName);
		forceExit("\nError: CSV file greater than max file size\n");
	}
	fseek(fileName, 0, SEEK_SET);
}


/**
 * @brief Counts amount of commas in a line
 *
 * @param line Pointer of a char array
 * @return int Number of commas found
 */
int commaCounter(char *line)
{
	int count = 0;
	for (int i = 0; i < strlen(line); i++) {
		if (line[i] == ',') {
			count++;
		}
	}
	return count;
}

/**
 * @brief Extracts index of NAME field from CSV
 * 
 * getNameIndex takes in a file pointer and returns
 * the index of the name position from the csv file
 *
 * getNameIndex uses strdup() and strsep() to parse lines.
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
int getNameIndex(FILE *fileName, int *quoted, int *comma)
{
	int loopCounter, index, foundName;
	loopCounter = index = foundName = 0;
	char buff[MAX_LINE + 1];
	char *str = strdup(fgets(buff, MAX_LINE + 1, fileName));
	*comma = commaCounter(str);
	if (strlen(str) == MAX_LINE) {
		fclose(fileName);
		free(str);
		forceExit("\nError: Exceeded max character length\n");
	}
	char *token = str, *end = str;
	char *nullCheck = NULL;
	while (token != NULL) {
		nullCheck = strsep(&end, ",");
		if (!nullCheck) {
			break;
		}
		if (strcmp(token, "name") == 0) {
			++foundName;
			if (foundName == 1) index = loopCounter;
		} else if (strcmp(token, "\"name\"") == 0) {
			++foundName;
			if (foundName == 1) index = loopCounter;
			*quoted = 1;
		}
		token = end;
		loopCounter++;
	}
	if (foundName > 1) {
		fclose(fileName);
		free(str);
		forceExit("\nError: More than one NAME column\n");
	} else if (foundName < 1) {
		fclose(fileName);
		free(str);
		forceExit("\nError: No NAME column found\n");
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
void processData(FILE *fileName, int namePos, Link *info, int *quoted, int *comma)
{
	int lineCount = 1;
	char buff[MAX_LINE + 1];
	while (!feof(fileName)) {
		if (lineCount > MAX_LINE) {
			freeLinkedMemory(info -> head, info);
			fclose(fileName);
			forceExit("\nError: CSV file greater than max line count\n");
		}
		char *str = fgets(buff, MAX_LINE + 1, fileName);
		if (!str) return;	// If EOF, return
		if (commaCounter(str) != *comma) {
			fclose(fileName);
			forceExit("\nError: Invalid input format -- wrong number of fields\n");
		} else if (strlen(str) >= MAX_CHAR) {
			// if line char count > max char count
			fclose(fileName);
			forceExit("\nError: Invalid input format -- too many characters in the line\n");
		}
		char *name = extractName(str, namePos, quoted, fileName, info);
		if (strcmp(name, "invalid") == 0) {
			fclose(fileName);
			forceExit("\nError: Invalid input format -- invalid name found\n");
		} else if (*name == '\0') {
			// If name field is empty string
			name = "empty";
		}
		insertToList(name, info);
		lineCount++;
	}
}

/**
 * @brief Extracts the name from CSV line given an index
 *
 * @param str Address to CSV line
 * @param namePos Index of NAME value in CSV line 
 * @param counter Address which contains count of commas
 * @return The supposed 'name' string at the index value
 */
char *extractName(char* str, int namePos, int *quoted, FILE *filename, Link *info)
{
	int index = 0;
	char *token = str, *end = str, *nameFound = NULL;
	char *nullCheck = NULL;
	while (token != NULL) {
		nullCheck = strsep(&end, ",");
		if (!nullCheck) return "invalid";
		if (index == namePos) {
			nameFound = token;
			if (*quoted == -1) checkQuotes(nameFound, filename, info);
			if (*quoted == 1) stripQuotes(nameFound, filename, info);
		}
		token = end;
		index++;
	}
	return nameFound;
 }

/**
 * @brief Checks if there're invalid quotes in NAME field
 * 
 * @param name Pointer to a char array representing NAME
 * @return void
 */
void checkQuotes(char *name, FILE *filename, Link *info)
{
	if (name[0] == '"' || name[strlen(name) - 1] == '"') {
		fclose(filename);
		forceExit("\nError: Invalid quotes in NAME field\n");
	}
}

/**
 * @brief Removes the outermost quotes level of a NAME string
 * 
 * @param name Pointer to a char array representing NAME
 * @return void
 */
void stripQuotes(char *name, FILE *filename, Link *info)
{
	if (strlen(name) < 2) {
		fclose(filename);
		forceExit("\nError: Invalid quotes in NAME field\n");
	} else if (name[0] != '"' || name[strlen(name) - 1] != '"') {
		fclose(filename);
		forceExit("\nError: Mismatching quotes in name field\n");
	}
	if (strlen(name) >= 3) {
		int len = strlen(name);
		strncpy(name, name + 1, len - 2);	
		for (int i = len; i > len - 3; i--) {
			removeChar(name, i);
		}
	} else {
		name = "";
	}
}

/**
 * @brief Removes a character from a given string by index
 * 
 * @param str Pointer of the string
 * @param index Index to be removed
 * @return void
 */
void removeChar(char *str, int index)
{
	char *src, *dest;
	int count = 0;
	for (src = dest = str; *src != '\0'; src++) {
		*dest = *src;
		if (count != index) dest++;
		count++;
	}
	*dest = '\0';
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
Node *createNode(int initial, Link *info)
{
	Node *newNode = malloc(sizeof(Node));
	if (newNode == NULL) {
		forceExit("\nError: Couldn't allocate memory -- New Node\n");
	}
	newNode -> next = NULL;
	newNode -> prev = NULL;
	Tweeter newTweeter;
	if (initial == 1) {
		// initial HEAD node
		newTweeter.count = 0;
	} else {
		newTweeter.count = 1;
	}
	newTweeter.name = NULL;
	newNode -> user = newTweeter;
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
		info -> head -> user.name = allocateName(name, info);
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
	Node *newNode = createNode(0, info);
	newNode -> user.name = allocateName(name, info);
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
char *allocateName(char *nameToCopy, Link *info)
{
	char *newName = malloc(sizeof(nameToCopy));
	if (newName == NULL) {
		forceExit("\nError: Couldn't allocate memory -- NAME field\n");
	}
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
		// print all elements
		if ((head -> user.name == NULL) && head -> user.count == 0) {
			return;
		}
		while (head != NULL) {
			printf("%s: %d\n", head -> user.name, head -> user.count);
			head = head -> next;
		}
		return;
	}
	// print top 10
	if ((head -> user.name == NULL) && head -> user.count == 0) {
		return;
	}
	for (int i = 0; i < count; i++) {
		if (head == NULL) break;
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
void freeLinkedMemory(Node *head, Link *info)
{
	Node *tmp;
	while (head != NULL) {
		tmp = head;
		head = head -> next;
		free(tmp -> user.name);
		free(tmp);
	}
	free(info);
}
