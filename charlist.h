#ifndef CHARLISTHEADER
#define CHARLISTHEADER

#include <stdlib.h>

//character list functions, allows arbitary length lines

typedef struct char_list {
	char c;
	struct char_list *next;
} char_list;

char_list *clmk(char c) ;

char_list *clappend(char_list *tail, char c) ;

void clfree(char_list *head) ;

unsigned int cllen(char_list *head) ;

char *clconvert(char_list *head) ;

#endif
