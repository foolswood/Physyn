#include <stdlib.h>

typedef struct actionlist {
	unsigned short (*act) (void*); //return 0 until it is to be destroyed, then return 1
	void *data;
	void (*free_data) (void*);
	struct actionlist *next;
} actionlist;

static actionlist *actions = NULL;
//actually need 3, before forces, after forces but before movement and after movement

void add_action(unsigned short (*act) (void*), void *data, void (*free_data) (void*)) {
	actionlist *new = malloc(sizeof(actionlist));
	new->act = act;
	new->data = data;
	new->free_data = free_data;
	new->next = actions;
	actions = new;
}

void do_actions(void) {
	actionlist *acting, *prev;
	acting = actions;
	prev = NULL;
	unsigned short rval;
	while (acting != NULL) {
		rval = (acting->act)((void*) acting->data);
		if (rval) { //destroy the acting action
			if (prev == NULL) { //first in list :. free memory and move the start of the list on
				(acting->free_data)((void*) acting->data);
				acting = actions->next;
				free(actions);
				actions = acting;
			} else { //in middle of list
				prev->next = acting->next;
				(acting->free_data)((void*) acting->data);
				free(acting);
				acting = prev->next;
			}
		} else { //move to the next item
			prev = acting;
			acting = acting->next;
		}
	}
}

//Testing Functions

#include "tempmodel.h"

typedef struct test_data {
	unsigned short i;
	vector v, x;
} test_data;

static unsigned short testing_samples = 2000;

unsigned short test_action(void *data) {
	test_data *td = data;
	(td->x)[1] = ((float) td->i) / testing_samples;
	(td->v)[1] = 0.0;
	(td->i)++;
	if (td->i == testing_samples) {
		return 1;
	}
	return 0;
}

void setup_test(temp_point *tree){
	test_data *td = malloc(sizeof(test_data));
	tree = find_pt(tree, "badboy");
	td->v = tree->fast->v;
	td->x = tree->fast->x;
	td->i = 0;
	add_action(&test_action, (void*) td, &free);
}
