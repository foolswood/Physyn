#ifndef ACTIONSHEADER
#define ACTIONSHEADER

#include <dlfcn.h>
#include "action_queue.h"
#include "linelist.h"
#include "tempmodel.h"
#include "parsingfuncs.h"

typedef struct ail {
	line_list *ll;
	char *action;
	struct ail *next;
} ail; //action initialization list


void register_action(char *action, line_list *head) ;

void free_ail(ail *a) ;

short init_actions(temp_point *tree) ;

#endif
