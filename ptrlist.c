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

//line list functions, allows the file to be an arbitary length

typedef struct line_list {
	char *str;
	struct line_list *next;
} line_list;

line_list *llmk(char *str) {
	line_list *new = malloc(sizeof(line_list));
	new->next = NULL;
	new->str = str;
	return new;
}

line_list *llappend(line_list *tail, char *str) {
	//appends a line to a line_list, returns new tail pointer
	line_list *new = llmk(str);
	tail->next = new;
	return new;
}

void llfree(line_list *head) {
	//frees a line_list
	line_list *next;
	while (head != NULL) {
		next = head->next;
		free(head->str);
		free(head);
		head = next;
	}
}
