#ifndef TEMPMODELHEADER
#define TEMPMODELHEADER

#include "vectors.h"
#include "points.h"
#include "springs.h"
#include <string.h>
#include <stdlib.h>
#include "model.h"

typedef struct temp_point {
	char *pt_id;
	float mass;
	float damping;
	vector position;
	unsigned int no_springs;
	point *fast;
	struct temp_point *left, *right;
} temp_point;

temp_point *mktPt(char *id, float mass, float damping, vector pos) ;

void free_tempoint(temp_point *a) ;

short add_point(temp_point **tree, char *id, float mass, float damping, vector pos) ;

void treemunch(temp_point *tree) ;

temp_point *find_pt(temp_point *tree, char *id) ;

typedef struct temp_spring {
	temp_point *a;
	unsigned int connection_no_a;
	temp_point *b;
	unsigned int connection_no_b;
	float restlength;
	float k;
	struct temp_spring *next;
} temp_spring;

temp_spring *add_spring(temp_point *tree, temp_spring *existing, char *idA, char *idB, float restlength, float k) ;

void listmunch(temp_spring *s) ;

unsigned int tree_items(temp_point *tree) ;


unsigned int listlen(temp_spring *springlist) ;

model convert(temp_point *tree, temp_spring *springlist) ;

#endif
