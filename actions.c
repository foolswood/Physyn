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

static ail *actions = NULL;

void register_action(char *action, line_list *head) {
	ail *new;
	if (actions == NULL) { //first action
		new = actions = malloc(sizeof(ail));
	} else { //otherwise append
		new = actions;
		while (new->next != NULL) {
			new = new->next;
		}
		new->next = malloc(sizeof(ail));
		new = new->next;
	}
	new->ll = head;
	new->action = action;
	new->next = NULL;
}

void free_ail(ail *a) {
	llfree(a->ll);
	free(a->action);
	free(a);
}

short init_actions(temp_point *tree) {
	ail *next;
	char *str;
	void *libhandle;
	void *(*fptr)(line_list*, temp_point*);
	while (actions != NULL) {
		//action module file path
		str = calloc(strlen(actions->action)+11, sizeof(char));
		strcpy(str, "actions/");
		strcat(str, actions->action);
		strcat(str, ".so");
		//load module
		libhandle = dlopen(str, RTLD_NOW);
		free(str);
		if (!libhandle) {
			printf("error in dlopen(%s)\n%s\n", str, dlerror());
			return 1;
		}
		//call action setup
		fptr = dlsym(libhandle, "setup_action"); //should probably be called init or something less wierd
		fptr(actions->ll, tree); //handle error conditions
		next = actions->next;
		free_ail(actions);
		actions = next;
	}
	return 0;
}
