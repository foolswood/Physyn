#include "points.h"
#include "springs.h"

typedef struct model {
	pointset *pts;
	springset *s;
	//actionset *a;
} model;

model mkmodel() {
	model m;
	m.pts = malloc(sizeof(pointset));
	m.s = malloc(sizeof(springset));
	return m;
}

void freemodel(model m) {
	free(m.pts);
	free(m.s);
}
