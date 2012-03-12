#ifndef MODELHEADER
#define MODELHEADER

#include "points.h"
#include "springs.h"

typedef struct model {
	pointset *pts;
	springset *s;
	//actionset *a;
} model;

model mkmodel() ;

void freemodel(model m) ;

#endif
