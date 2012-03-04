// A simple inverse square mic
#include "../parsingfuncs.h"
#include "../linelist.h"
#include "../tempmodel.h"
#include "../vectors.h"

typedef struct mic {
	//Ideally these would be const vector
	vector pv; //the velocity vector to watch
	vector dv; //the direction vector
} mic;

float get_out(void* ptr) { //get the output from the microphone
	mic const *m = ptr;
	return Vdot(m->pv, m->dv);
}

void *init(line_list *head, temp_point *tree) { //read a section of the config file to get the data to initialize the mic
	float s;
	unsigned int i = 0;
	vector v;
	int rval; //used to store a return value
	mic *m = malloc(sizeof(mic));
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
	m->pv = tree->fast->v;
	m->dv = v;
	return (void*) m;
}

void free_data(void* ptr) {//free any heap data belonging to the mic
	mic *m = ptr;
	free(m->dv);
	free(m);
}
