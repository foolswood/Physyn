#ifndef PARSINGFUNCSHEADER
#define PARSINGFUNCSHEADER

#include "linelist.h"
#include "charlist.h"
#include <stdlib.h>
#include <ctype.h>
#include "vectors.h"

char *get_word(const char *str, unsigned int *pos) ;


vector read_vector(const char *line, unsigned int *pos) ;

short extract_curly_braces(line_list **hd, line_list **sl, unsigned int * const i) ;

#endif
