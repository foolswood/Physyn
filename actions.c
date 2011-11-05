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

//Test Action (move) is living here temporarily before I split it out into a loading module.
#include "vectors.h"

typedef struct move_data {
	vector x, loc;
} move_data;

unsigned short do_action(void *data) {
	move_data *md = (move_data*) data;
	Vadd(md->x, md->x, md->loc);
	return 1;
}

void action_free_data(void *data) {
	move_data *md = (move_data*) data;
	free(md->loc);
	free(md);
}

short setup_action(line_list *ll, temp_point *tree) {
	unsigned int i = 0;
	vector pos;
	move_data *md;
	char *w = get_word(ll->str, &i);
	tree = find_pt(tree, w);
	//test if the point exists
	free(w);
	pos = read_vector(ll->str, &i);
	//test if vector exists
	md = malloc(sizeof(move_data));
	md->x = tree->fast->x;
	md->loc = pos;
	//give data and functions to handler
	add_action(ACT_END, &do_action, (void*) md, &action_free_data);
	return 0; //do some error conditions
}

//End of Test Action

short init_actions(temp_point *tree) {
	ail *next;
	while (actions != NULL) {
		//load action module
		//setup action
		setup_action(actions->ll, tree); //handle error conditions
		next = actions->next;
		free_ail(actions);
		actions = next;
	}
	return 0;
}
