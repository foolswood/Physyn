#ifndef LOADMODELHEADER
#define LOADMODELHEADER

#include "linelist.h"
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include "model.h"
#include "tempmodel.h"
#include "readfile.h"
#include "parsingfuncs.h"
#include "capture.h"
#include "actions.h"

void get_dimensions(line_list **hd) ; //improve error messages

unsigned int pts_sp(line_list *head, temp_point **tree, temp_spring **sp) ; //returns the no of points, 0 is error

model fileload(char *path) ;

#endif
