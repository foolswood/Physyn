// A simple inverse square mic
#include "../loadconf.h"
#include "../ptrlist.h"
#include "../tempmodel.h"
#include "../vectors.h"
#include "plugin_header.h"

typedef struct mic {
	const vector pv; //the velocity vector to watch
	const vector dv; //the direction vector
} mic;

static float mic_out(void* ptr) { //get the output from the microphone
	mic const *m = ptr;
	return Vdot(m->pv, m->dv);
}

static inline capture_device *mk_cd(const vector pv, const vector dv) {
	mic *p = malloc(sizeof(mic));
	capture_device *c = malloc(sizeof(capture_device));
	mic m = {pv, dv};
	memcpy(p, &m, sizeof(mic));
	c->get_output = &mic_out;
	c->data = p;
	return c;
}

capture_device *init(line_list *head, temp_point *tree) { //read a section of the config file to get the data to initialize the mic
	float s;
	unsigned int i = 0;
	vector v;
	int rval; //used to store a return value
	//Get the point the config refers to
	char *w = get_word(head->str, &i);
	tree = find_pt(tree, w);
	if (tree == NULL) {
		printf("line %i: %s is not a valid point identifier\n", head->lineno, w);
		free(w);
		return NULL;
	}
	free(w);
	//Get the direction vector to look in
	v = read_vector(head->str, &i);
	if (v == NULL) {
		printf("line %i: error, vector expected\n", head->lineno);
		free(v);
		return NULL;
	}
	//Get the scale value
	rval = sscanf((head->str)+i, "%f", &s);
	if (rval != 1) {
		printf("line %i: invalid scale value\n", head->lineno);
		free(v);
		return NULL;
	}
	Vtimes_scalar(v, v, s);
	return mk_cd(tree->fast->v, v);
}

void free_data(void* ptr) {//free any heap data belonging to the mic
	mic *m = ptr;
	free(m->dv);
	free(m);
}
