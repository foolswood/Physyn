#include "vectors.h"
#include "points.h"
#include <stdlib.h>

typedef struct spring {
	point *a, *b;
	unsigned int a_no, b_no;
	float restlength, k;
} spring;

typedef struct springset {
	unsigned int no;
	spring *springs;
} springset;

inline void apply_spring(spring *s, vector v) {
	float d;
	Vsub(v, (s->a)->x, (s->b)->x);
	d = Vmodulus(v);
	d = (s->k)*((d - s->restlength)/s->restlength);
	Vtimes_scalar((s->a)->F[s->a_no], v, -d);
	Vtimes_scalar((s->b)->F[s->b_no], v, d);
}

void apply_springs(springset s) {
	static unsigned int i;
	vector v = Vmk();
	for (i=0; i<(s.no); i++) {
		apply_spring((s.springs)+i, v);
	}
	free(v);
}
