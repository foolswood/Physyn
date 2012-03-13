#include "loadmodel.h"
#include "io.h"
#include "springs.h"
#include "points.h"
#include "capture.h"
#include "action_queue.h"
#include <stdio.h>

springset ss;
pointset ps;

void fileload(char *path) {
	temp_point *tree = NULL;
	temp_spring *springs = NULL;
	unsigned int no_points, no_springs;
	line_list *ll = readfile(path); //read input file
	get_dimensions(&ll);
	no_points = pts_sp(ll, &tree, &springs); //initial pass for relationships
	no_springs = listlen(springs);
	if (no_springs && no_points) {
		convert(tree, &ps, springs, &ss); //could make use of knowing springs and node numbers, since this works it out again
		init_actions(tree);
		init_capture(tree);
	}
	else
		printf("%s\n", "no points or no springs defined");
	treemunch(tree);
	listmunch(springs);
}

short rtloop(void) {
	static int i;
	short (*pull)(void) = io_func("pull");
	for (i=0; i<10000000; i++) {
		(*pull)();
		do_actions(ACT_START);
		apply_springs(ss);
		do_actions(ACT_MIDDLE);
		apply_forces(ps);
		do_actions(ACT_END);
		get_output();
	}
	return 0;
}

int main(void) {
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
	fileload("trial.pts");
	//prepare to simulate
	io_go = io_func("go");
	(*io_go)();
	//simulate
	rtloop();
	//do an atexit() thing
	//free everything
	free_capture();
	return 0;
}
