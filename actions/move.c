#include "../action_queue.h"
#include "../linelist.h"
#include "../tempmodel.h"
#include "../parsingfuncs.h"
#include "../vectors.h"
#include "../midi.h"
#include "../midi_helpers.h"

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
	move_data *d, *md;
	unsigned short velocity;
	if ((MIDI_EVENT & buffer[0]) == MIDI_NOTE_ON) {
		velocity = buffer[2];
		md = malloc(sizeof(move_data));
		d = (move_data*) data;
		md->x = d->x;
		md->loc = Vmk();
		Vtimes_scalar(md->loc, d->loc, (velocity + 1)/128.0);
		add_action(ACT_START, &do_action, (void*) md, action_free_data);
	}
	return 0;
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
	register_midi_handler(&on_midi, (void*) md);
	return 0; //do some error conditions
}
