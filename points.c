#include "vectors.h"
#include <math.h>
#include <stdlib.h>

typedef struct point {
	vector x, v, *F;
	unsigned short connections;
	float b, mass;
} point;

typedef struct pointset {
	unsigned int no;
	point *pts;
} pointset;

float frame_time = 1.0/44100;

point *mk_point(float b, float mass, vector a, unsigned short springs) {
	vector x = Vheap(a), v = Vmk();
	vector *F = calloc(springs, sizeof(vector*));
	unsigned int i;
	Vzero(v);
	for (i=0; i<springs; i++) {
		F[i] = Vmk();
		Vzero(F[i]);
	}
	point *p = malloc(sizeof(point));
	p->x = x;
	p->v = v;
	p->F = F;
	p->b = b;
	p->mass = mass;
	p->connections = springs;
	return p;
}

void free_point(point *p) {
	free(p->x);
	free(p->v);
	free(p->F);
	free(p);
}

inline void apply_force(point *a) {
	unsigned short d;
	if (isinf(a->mass)) {
	}
	else {
		for (d = 1; d < a->connections; d++)
			Vadd(a->F[0], a->F[0], a->F[d]);
		for (d = 0; d < dimensions; d++) {
			a->v[d] += a->F[0][d]*frame_time*0.5/a->mass;
			a->v[d] = a->v[d]*a->b;
			a->x[d] += a->v[d]*frame_time;
		}
	}
}

void apply_forces(pointset a) {
	unsigned int p;
	for (p = 0; p < a.no; p++) {
		apply_force((a.pts)+p);
	}
}

/*
int runtest(void) {
	point a;
	float n[] = {0.0, 0.0};
	a.x = n;
	float m[] = {0.0, 0.0};
	a.v = m;
	float l[] = {1.0, 0.0};
	a.F = l;
	a.b = 1.0;
	a.mass = 1.0;
	pointset b = {1, &a};
	apply_forces(&b);
	Vprint(a.x);
	return 0;
}
*/
