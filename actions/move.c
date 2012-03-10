#include "../action_queue.h"
#include "../linelist.h"
#include "../tempmodel.h"
#include "../parsingfuncs.h"
#include "../vectors.h"
#include "../io.h"

//Test Action (move) - Should be a dynamic loaded module

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

short on_midi(unsigned char* buffer, size_t size, void *data) {
	add_action(ACT_START, &do_action, data, NULL);
	return 0;
}

short setup_action(line_list *ll, temp_point *tree) {
	unsigned int i = 0;
	vector pos;
	move_data *md;
	char *w = get_word(ll->str, &i);
	void (*cmi)(char*, short (*mh)(unsigned char*, size_t, void*), void *hd) = io_func("create_midi_in");
	tree = find_pt(tree, w);
	//test if the point exists
	free(w);
	pos = read_vector(ll->str, &i);
	//test if vector exists
	md = malloc(sizeof(move_data));
	md->x = tree->fast->x;
	md->loc = pos;
	//give data and functions to handler
	(*cmi)("move", &on_midi, (void*) md);
	return 0; //do some error conditions
}
