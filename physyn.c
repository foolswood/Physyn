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
	unsigned int rate;
	int i;
	m = fileload("trial.pts");
	rate = out_init();
	if (rate == 0) {
		return 1;
	}
	rtloop(m);
	jack_go();
	for (i=0; i<1024; i++) {
		rtloop(m);
	}
	return 0;
}
