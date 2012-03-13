#ifndef SPRINGSHEADER
#define SPRINGSHEADER

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

inline void apply_spring(spring *s, vector v) ;

void apply_springs(springset s) ;

#endif
