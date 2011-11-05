#include "linelist.h"
#include "charlist.h"
#include <stdio.h>
//#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include "vectors.h"

char *get_word(const char *str, unsigned int *pos) {
	unsigned int i = *pos;
	char c = str[i];
	char_list *tail, *cl;
	while (isspace(c))
		c = str[++i];
	if (c == '\0')
		return NULL;
	if ((!isspace(c)) && (c != '\0')) {
		cl = tail = clmk(c);
		c = str[++i];
	}
	else
		return NULL;
	while ((!isspace(c)) && (c != '\0')) {
		tail = clappend(tail, c);
		c = str[++i];
	}
	*pos = i;
	return clconvert(cl);
}

static short is_float_char(char c) {
	switch (c) {
		case '.':
		case '-':
		case '+':
		case 'e':
		case 'E':
			break;
		default:
			if (!isdigit(c))
				return 0;
	}
	return 1;
}

vector read_vector(const char *line, unsigned int *pos) {
	unsigned int i, start, d;
	vector v = Vmk();
	i = *pos;
	d = 0;
	while (isspace(line[i]))
		if (line[++i] == '\0') { //line ends before first bracket
			free(v);
			return NULL;
		}
	if (line[i++] == '(') {
		while (d < dimensions) {
			start = i;
			while (is_float_char(line[i]) || isspace(line[i])) //count through the float
				if (line[++i] == '\0') {
					free(v);
					return NULL;
				}
			if (line[i] == ',') {
				if (sscanf(line+start, "%f", v+(d++))) //read an entry
					i++;
				else
					break;
			}
			else {
				if (line[i == ')']) {
					if ((sscanf(line+start, "%f", v+(d++))) && (d == dimensions)) { //read entry and check full
						*pos = ++i; //move the counter onward
						return v;
					}
					else
						break;
				}
			}
		}
	}
	free(v);
	return NULL;
}

short extract_curly_braces(line_list **hd, line_list **sl, unsigned int * const i) {
	//hd is the initial list head ptr, sl is for the new list, i is an offset, returns 0 if successful!
	//for some reason this function is vile, if you can tidy it, do
	line_list *before, *start, *end, *head = *hd;
	char c, *str;
	unsigned int j, bcounter =  1;
	if (head == NULL)
		return 4;
	while (1) { //find 1st bracket
		c = head->str[*i];
		if (isspace(c)) {
			(*i)++;
		}
		else {
			if (c == '{')
				break;
			else {
				if (c == '\0') {
					head = head->next;
					if (head == NULL)
						return 1; //no open bracket
					(*i) = 0;
				}
				else
					return 2; //something before open bracket
			}
		}
	}
	if (head == *hd) {
		j = *i;
		do {
			c = head->str[--j];
		} while (isspace(c));
		j++;
		str = calloc(j+1, sizeof(char));
		strncpy(str, head->str, j*sizeof(char));
		str[j] = '\0';
		start = head->next; //guessing ahead at this point
		before = llappend(head->prev, str, head->lineno);
		*hd = before;
		j = *i;
		do {
			c = head->str[++j];
		} while(isspace(c));
		if (c != '\0') {
			str = calloc(strlen(head->str)-j, sizeof(char));
			strncpy(str, (head->str)+j, (strlen(head->str)-j)*sizeof(char));
			end = llmk(str, head->lineno);
			end->next = start;
			start->prev = end;
			start = end;
		}
		else {
			start->prev = NULL;
		}
		free((char*) head->str);
		free(head);
	}
	else {
		before = *hd;
		start = head;
		if (start->prev != before) { //there are blank lines between the call and the open bracket
			printf("memory leaking here, either fix your wierd ass config file or give me a prod and I'll fix this; fools\n");
		}
	}
	*sl = start;
	*i = 0;
	end = start;
	while (1) {
		c = end->str[*i];
		switch (c) {
			case '{':
				bcounter++;
				(*i)++;
				break;
			case '}':
				bcounter--;
				(*i)++;
				break;
			case '\0':
				end = end->next;
				*i = 0;
				if (end == NULL)
					return 3; //no opposing bracket
				break;
			default:
				(*i)++;
				break;
		}
		if (bcounter == 0)
			break;
	}
	//line with close bracket
	j = *i;
	do {
		c = (end->str)[j++];
	} while(isspace(c));
	if (c == '\0') {
		if (end->next == NULL)
			before->next = NULL;
		else {
			end->next->prev = before;
			before->next = end->next;
			end->next = NULL;
		}
	}
	else {
		j = (*i) + 1;
		while (isspace((end->str)[j++])) {
		}
		str = calloc(strlen(end->str) - j, sizeof(char));
		strncpy(str, end->str, strlen(end->str) - j - 2);
		str[strlen(end->str) - j - 2] = '\0';
		head = llmk(str, end->lineno);
		head->prev = before;
		before->next = head;
		head->next = end->next;
	}
	j = (*i) - 1;
	while (j) {
		if (!isspace((end->str)[j--])) {
			str = calloc(j+2, sizeof(char));
			strncpy(str, end->str, j+1);
			str[j+1] = '\0';
			head = llmk(str, end->lineno);
			end->prev->next = head;
			head->prev = end->prev;
			break;
		}
	}
	if (!j)
		end->prev->next = NULL;
	free((char*) end->str);
	free(end);
	return 0;
}
