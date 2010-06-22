// A simple inverse square mic
#include "../loadconf.h"
#include "../ptrlist.h"
#include "../tempmodel.h"

typedef struct mic {
	const vector v; //the velocity vector to watch
	const float scale; //multiplier to allow scaling of the output (like an internal gain slider) to get it within the range
	float (*get) (struct mic m); //call this to get the output
} mic;

static float mic_out(const mic m) { //get the output from the microphone
	return Vmodulus(m.v);
}

static inline mic *mk_mic(const vector v, const float scale) {
	mic *p = malloc(sizeof(mic));
	mic m = {v, scale, &mic_out};
	memcpy(p, &m, sizeof(mic));
	return p;
}

mic *init(line_list *head, temp_point *tree) { //read a section of the config file to get the data to initialize the mic
	float s;
	unsigned int i = 0;
	char *w = get_word(head->str, &i);
	tree = find_pt(tree, w);
	if (tree == NULL) {
		printf("line %i: %s is not a valid point identifier\n", head->lineno, w);
		free(w);
		return 0;
	}
	if (!sscanf((head->str)+i, "%f", &s)) {
		printf("line %i: invalid scale value\n", head->lineno);
		free(w);
		return 0;
	}
	return mk_mic(tree->fast->v, s);
}
