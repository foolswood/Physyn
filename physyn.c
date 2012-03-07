#include "loadmodel.h"
#include "io.h"
#include "springs.h"
#include "points.h"
#include "capture.h"
#include "action_queue.h"
#include <stdio.h>

//move load file functions to here

short rtloop(model m) {
	static int i;
	for (i=0; i<100000; i++) {
		do_actions(ACT_START);
		apply_springs(m.s);
		do_actions(ACT_MIDDLE);
		apply_forces(*(m.pts));
		do_actions(ACT_END);
		get_output();
	}
	return 0;
}

int main(void) {
	model m;
	unsigned int rate;
	void (*io_go)(void);
	//parse arguments
	//load IO
	rate = load_io("jack"); //eventually cater for more
	if ( rate == 0 ) {
		printf("Exiting: Could Not Setup IO\n");
		return 1;
	}
	//load file
	m = fileload("trial.pts");
	//prepare to simulate
	io_go = io_func("go");
	(*io_go)();
	//simulate
	rtloop(m);
	//free everything
	free_capture();
	return 0;
}
