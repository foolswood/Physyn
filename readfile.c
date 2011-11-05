#include <stdio.h>
#include <stdlib.h>
#include "linelist.h"
#include "charlist.h"

static char *get_line(FILE *f) {
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

line_list *readfile(const char * const path) { //attach line nos so the errors are easier to find
	//returns a line_list containing all valid data from the file, returns NULL if the file cannot be opened or there is no data in the file
	unsigned int lineno = 0;
	FILE *f = fopen(path, "r");
	line_list *head, *tail;
	head = tail = NULL;
	char *line;
	if (f == NULL)
		return NULL;
	line = get_line(f);
	while (line != NULL) {
		lineno++;
		if (strlen(line) == 0) { //do not add empty lines
			free(line);
			line = get_line(f);
		}
		else {
			head = tail = llmk(line, lineno);
			line = get_line(f);
			break;
		}
	}
	while (line != NULL) {
		lineno++;
		if (strlen(line) == 0)
			free(line);
		else {
			tail = llappend(tail, line, lineno);
		}
		line = get_line(f);
	}
	fclose(f);
	return head;
}
