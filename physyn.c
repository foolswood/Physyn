#include "loadmodel.h"
#include "out_channels.h"
#include "springs.h"
#include "points.h"
#include "capture.h"

short rtloop(model m) {
	static int i;
	for (i=0; i<1024; i++) {
		apply_springs(m.s);
		apply_forces(*(m.pts));
		get_output();
	}
	return 0;
}

int main(void) {
	model m;
	out_init();
	m = fileload("trial.pts");
	rtloop(m);
	jack_go();
	int i;
	for (i=0; i<1024; i++) {
		rtloop(m);
	}
	return 0;
}
