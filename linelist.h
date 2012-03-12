#ifndef LINELISTHEADER
#define LINELISTHEADER

#include <stdlib.h>
#include <string.h>
#include "charlist.h"

//line list functions, allows the file to be an arbitary length

typedef struct line_list {
	const char * const str;
	const unsigned int lineno;
	struct line_list *next, *prev;
} line_list;

line_list *llmk(const char * const str, const unsigned int lineno) ;

line_list *llappend(line_list * const tail, const char *str, const unsigned int lineno) ;

/*
unsigned short *llsplit(line_list *head, line_list *splitpoint) ;

void *lljoin(line_list *a, line_list *b) ;
*/

void llfree(line_list *head) ;

#endif
