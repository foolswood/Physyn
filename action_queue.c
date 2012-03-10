#include <stdlib.h>

typedef struct actionlist {
	unsigned short (*act) (void*); //return 0 until it is to be destroyed, then return 1
	void *data;
	void (*free_data) (void*);
	struct actionlist *next;
} actionlist;

typedef enum act_when {
	ACT_START,
	ACT_MIDDLE,
	ACT_END
}act_when;

static actionlist *acting[3] = {NULL, NULL, NULL}; //correspond to the things in the enum

void add_action(int when, unsigned short (*act) (void*), void *data, void (*free_data) (void*)) {
	actionlist *new = malloc(sizeof(actionlist));
	new->act = act;
	new->data = data;
	new->free_data = free_data;
	new->next = acting[when];
	acting[when] = new;
}

void do_actions(int when) {
	actionlist *active, *prev;
	active = acting[when];
	prev = NULL;
	unsigned short rval;
	while (active != NULL) {
		rval = (active->act)((void*) active->data);
		if (rval) { //destroy the acting action
			if (prev == NULL) { //first in list :. free memory and move the start of the list on
				if (active->free_data != NULL)
					(active->free_data)((void*) active->data);
				active = (acting[when])->next;
				free(acting[when]);
				acting[when] = active;
			} else { //in middle of list
				prev->next = active->next;
				if (active->free_data != NULL)
					(active->free_data)((void*) active->data);
				free(active);
				active = prev->next;
			}
		} else { //move to the next item
			prev = active;
			active = active->next;
		}
	}
}
