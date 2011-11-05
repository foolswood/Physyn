#include <stdlib.h>
#include <string.h>
#include "charlist.h"

//line list functions, allows the file to be an arbitary length

typedef struct line_list {
	const char * const str;
	const unsigned int lineno;
	struct line_list *next, *prev;
} line_list;

line_list *llmk(const char * const str, const unsigned int lineno) {
	line_list new = {str, lineno, NULL, NULL};
	line_list *n = malloc(sizeof(line_list));
	memcpy(n, &new, sizeof(line_list));
	return n;
}

line_list *llappend(line_list * const tail, const char *str, const unsigned int lineno) {
	//appends a line to a line_list, returns new tail pointer
	line_list *new = llmk(str, lineno);
	tail->next = new;
	new->prev = tail;
	return new;
}

/*
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
*/

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
