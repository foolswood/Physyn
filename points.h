#ifndef POINTSHEADER
#define POINTSHEADER

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

extern float frame_time ;

point *mk_point(float b, float mass, vector a, unsigned short springs) ;

void free_point(point *p) ;

inline void apply_force(point *a) ;

void apply_forces(pointset a) ;

#endif
