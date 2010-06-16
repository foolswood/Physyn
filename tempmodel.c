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

temp_point *mktPt(char *id, float mass, float damping, vector pos) {
	temp_point *new = malloc(sizeof(temp_point));
	new->pt_id = strdup(id);
	new->mass = mass;
	new->damping = damping;
	new->position = Vheap(pos);
	new->no_springs = 0;
	new->fast = NULL;
	new->left = new->right = NULL;
	return new;
}

void free_tempoint(temp_point *a) {
	free(a->pt_id);
	free(a->position);
	free(a);
}

short add_point(temp_point **tree, char *id, float mass, float damping, vector pos) {
	temp_point *p = *tree;
	while (p != NULL) {
		short c = strcmp(p->pt_id, id);
		switch (c) {
			case -1: tree = &(p->left);
				break;
			case 1: tree = &(p->right);
				break;
			case 0: return 0;
		}
		p = *tree;
	}
	*tree = mktPt(id, mass, damping, pos);
	return 1;
}

void treemunch(temp_point *tree) {
	if (tree == NULL)
		free(tree);
	else {
		if (tree->left == NULL)
			treemunch(tree->left);
		if (tree->right == NULL)
			treemunch(tree->right);
		free(tree);
	}
}

temp_point *find_pt(temp_point *tree, char *id) {
	while (tree != NULL) {
		switch (strcmp(tree->pt_id, id)) {
			case -1:
				tree = tree->left;
				break;
			case 1:
				tree = tree->right;
				break;
			case 0:
				return tree;
		}
	}
	return NULL;
}

typedef struct temp_spring {
	temp_point *a;
	unsigned int connection_no_a;
	temp_point *b;
	unsigned int connection_no_b;
	float restlength;
	float k;
	struct temp_spring *next;
} temp_spring;

temp_spring *add_spring(temp_point *tree, temp_spring *existing, char *idA, char *idB, float restlength, float k) {
	temp_spring *new;
	temp_point *a = find_pt(tree, idA), *b = find_pt(tree, idB);
	if ((a != NULL) && (b != NULL)) {
		new = malloc(sizeof(temp_spring));
		new->a = a;
		new->b = b;
		new->connection_no_a = (a->no_springs)++;
		new->connection_no_b = (b->no_springs)++;
		new->restlength = restlength;
		new->k = k;
		new->next = existing;
		return new;
	} else
		return NULL;
}

void listmunch(temp_spring *s) {
	temp_spring *next;
	while ((next = s->next))
		free(s);
}

unsigned int tree_items(temp_point *tree) {
	unsigned int counter = 1; //for self
	if (tree == NULL)
		return 0;
	counter += tree_items(tree->left);
	counter += tree_items(tree->right);
	return counter;
}

unsigned int mk_fast_pts(temp_point *tree, point *pts, unsigned int i) {
	unsigned int j;
	if (tree->left != NULL)
		i = mk_fast_pts(tree->left, pts, i);
	if (tree->right != NULL)
		i = mk_fast_pts(tree->right, pts, i);
	point *p = pts+i;
	p->x = tree->position;
	p->v = Vmk();
	p->F = calloc(tree->no_springs, sizeof(vector*));
	for (j = 0; j < tree->no_springs; j++)
		(p->F)[j] = Vmk();
	p->connections = tree->no_springs;
	p->mass = tree->mass;
	p->b = tree->damping;
	tree->fast = p;
	i++;
	return i;
}

unsigned int listlen(temp_spring *springlist) {
	unsigned int counter = 0;
	while (springlist != NULL) {
		counter++;
		springlist = springlist->next;
	}
	return counter;
}

model convert(temp_point *tree, temp_spring *springlist) {
	model m = mkmodel();
	unsigned int s, i;
	spring *sp;
	temp_spring *next;
	s = tree_items(tree);
	m.pts->no = s;
	m.pts->pts = (point*) calloc(s, sizeof(point));
	//traverse tree creating points
	mk_fast_pts(tree, m.pts->pts, 0); //also fills the points array
	//create springs with links to those points
	s = listlen(springlist);
	m.s->no = s;
	sp = m.s->springs = calloc(s, sizeof(spring));
	for (i=0; i < s; i++) {
		//move the important bits from the list to the array
		(sp[i]).a = (springlist->a)->fast;
		(sp[i]).a_no = springlist->connection_no_a;
		(sp[i]).b = (springlist->b)->fast;
		(sp[i]).b_no = springlist->connection_no_b;
		//make the occasional use vector
		(sp[i]).v = Vmk();
		//advance through the list and remove temp springs
		next = springlist->next;
		free(springlist);
		springlist = next;
	}
	treemunch(tree);
	return m;
}
