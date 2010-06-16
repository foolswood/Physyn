#include "vectors.h"
#include "points.h"
#include <stdlib.h>

typedef struct spring {
	point *a, *b;
	unsigned int a_no, b_no;
	float restlength, k;
	vector v; //this is a temporary storage vector used during evaluation
} spring;

typedef struct springset {
	unsigned int no;
	spring *springs;
} springset;

inline void apply_spring(spring *s) {
	Vsub(s->v, (s->a)->x, (s->b)->x);
	float d = Vmodulus(s->v);
	d = (s->k)*(d - s->restlength)/d;
	Vtimes_scalar((s->a)->F[s->a_no], s->v, d);
	Vtimes_scalar((s->b)->F[s->b_no], s->v, -d);
}

void apply_springs(springset *s) {
	unsigned int i;
	for (i=0; i<(s->no); i++) {
		apply_spring((s->springs)+i);
	}
}
