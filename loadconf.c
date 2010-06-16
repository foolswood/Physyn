#include "ptrlist.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include "vectors.h"

char *get_line(FILE *f) {
	//read one line from the file f, returns a string, unless the end of file is reached, in which case returns NULL
	int c = fgetc(f);
	char_list *head = NULL, *tail;
	if (c == EOF)
		return NULL;
	while ((c != '\n') && (c != EOF)) {
		if (c == '#') { //anything after a comment on a line is ignored
			while ((c != '\n') && (c != EOF))
				c = fgetc(f);
			break;
		}
		if (head == NULL) //first character
			head = tail = clmk((char) c);
		else
			tail = clappend(tail, (char) c);
		c = fgetc(f);
	}
	return clconvert(head); //to a C style string
}

line_list *readfile(char *path) { //attach line nos so the errors are easier to find
	//returns a line_list containing all valid data from the file, returns NULL if the file cannot be opened or there is no data in the file
	FILE *f = fopen(path, "r");
	line_list *head, *tail;
	head = tail = NULL;
	char *line;
	if (f == NULL)
		return NULL;
	line = get_line(f);
	while (line != NULL) {
		if (strlen(line) == 0) { //do not add empty lines
			free(line);
			line = get_line(f);
		}
		else {
			head = tail = llmk(line);
			line = get_line(f);
			break;
		}
	}
	while (line != NULL) {
		if (strlen(line) == 0)
			free(line);
		else
			tail = llappend(tail, line);
		line = get_line(f);
	}
	fclose(f);
	return head;
}

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

short is_float_char(char c) {
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

vector read_vector(char *line, unsigned int *pos) {
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
