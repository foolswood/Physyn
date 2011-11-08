#include "loadmodel.h"
#include "jack_interface.h"
#include "springs.h"
#include "points.h"
#include "capture.h"
#include "action_queue.h"
#include <stdio.h>

short rtloop(model m) {
	static int i;
	for (i=0; i<1024; i++) {
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
	int i;
	rate = jack_init();
	m = fileload("trial.pts");
	if ( rate == 0 ) {
		printf("Exiting: Could Not Create Jack Client\n");
		return 1;
	}
	rtloop(m);
	jack_go();
	for (i=0; i<1024; i++) {
		rtloop(m);
	}
	free_capture();
	return 0;
}
