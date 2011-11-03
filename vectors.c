#include <math.h>
#include <stdlib.h>
#include <stdio.h>

unsigned int dimensions; //const - this is for the benifit of the header generator

short set_dimensions(unsigned int i) {
	static int runbefore;
	if (!runbefore) {
		dimensions = i;
		runbefore = 1;
	}
	else {
		printf("Shan't! (you tried to set the dimensions twice, bad)\n");
		dimensions = 0;
	}
	return dimensions;
}

typedef float* vector;

inline vector Vmk(void) {
	return calloc(dimensions, sizeof(float));
}

vector Vheap(const vector const a) {
	vector v = Vmk();
	unsigned short d;
	for (d=0; d < dimensions; d++) {
		v[d] = a[d];
	}
	return v;
}


void Vzero(vector const a) {
	unsigned short d;
	for (d = 0; d < dimensions; d++) {
		a[d] = 0.0;
	}
}


void Vadd(vector const a, const vector const b, const vector const c) {
	unsigned short d;
	for (d = 0; d < dimensions; d++)
		a[d] = b[d] + c[d];
}

void Vsub(vector const a, const vector const b, const vector const c) {
	unsigned short d;
	for (d = 0; d < dimensions; d++)
		a[d] = b[d] - c[d];
}


void Vtimes_scalar(vector const a, const vector const b, const float c) {
	unsigned short d;
	for (d = 0; d < dimensions; d++) {
		a[d] = b[d]*c;
	}
}

float Vdot(const vector const a, const vector const b) {
	float sum = 0.0;
	unsigned short d;
	for (d = 0; d < dimensions; d++) {
		sum += a[d]*b[d];
	}
	return sum;
}

float Vmodulus(const vector const a) {
	unsigned short d;
	float sum = 0.0;
	for (d = 0; d < dimensions; d++) {
		sum += pow(a[d], 2.0);
	}
	return sqrt(sum);
}

void Vprint(const vector const a) {
	unsigned short d;
	for (d = 0; d < dimensions; d++) {
		printf("%f ", a[d]);
	}
	printf("\n");
}
