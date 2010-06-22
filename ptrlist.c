#include <stdlib.h>
#include <string.h>

//character list functions, allows arbitary length lines

typedef struct char_list {
	char c;
	struct char_list *next;
} char_list;

char_list *clmk(char c) {
	char_list *new = malloc(sizeof(char_list));
	new->c = c;
	new->next = NULL;
	return new;
}

char_list *clappend(char_list *tail, char c) {
	//appends to the character list, returns a new tail pointer
	char_list *new = clmk(c);
	tail->next = new;
	return new;
}

void clfree(char_list *head) {
	//frees character list
	char_list *next;
	while (head != NULL) {
		next = head->next;
		free(head);
		head = next;
	}
}

unsigned int cllen(char_list *head) {
	//returns the length of a character list
	unsigned int a = 0;
	while (head != NULL) {
		a++;
		head = head->next;
	}
	return a;
}

char *clconvert(char_list *head) {
	//converts a character list to a C style string
	unsigned int i = 0;
	char *str = calloc(cllen(head)+1, sizeof(char*));
	while (head != NULL) {
		str[i++] = head->c;
		head = head-> next;
	}
	clfree(head);
	str[i] = '\0';
	return str;
}

//line list functions, allows the file to be an arbitary length

typedef struct line_list {
	const char * const str;
	const unsigned int lineno;
	struct line_list *next;
} line_list;

line_list *llmk(const char * const str, const unsigned int lineno) {
	line_list new = {str, lineno, NULL};
	line_list *n = malloc(sizeof(line_list));
	memcpy(n, &new, sizeof(line_list));
	return n;
}

line_list *llappend(line_list * const tail, const char *str, const unsigned int lineno) {
	//appends a line to a line_list, returns new tail pointer
	line_list *new = llmk(str, lineno);
	tail->next = new;
	return new;
}

unsigned short *llsplit(line_list *head, line_list *splitpoint) {
	//splits a line list at the specified point
	if (head == NULL)
		return 0;
	while (head->next != splitpoint) {
		if (head->next == NULL)
			return 0;
		head = head->next;
	}
	head->next = NULL;
	return 1;
}

void *lljoin(line_list *a, line_list *b) {
	//append list b to list a
	if (a == NULL)
		a = b;
	else {
		if (b != NULL) {
			while (a->next != NULL)
				a = a->next;
			a->next = b;
		}
	}
}

void llfree(line_list *head) {
	//frees a line_list
	line_list *next;
	while (head != NULL) {
		next = head->next;
		free((char*) head->str);
		free(head);
		head = next;
	}
}
