#include <stdlib.h>

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
