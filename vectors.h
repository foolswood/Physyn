#ifndef VECTORSHEADER
#define VECTORSHEADER

#include <math.h>
#include <stdlib.h>
#include <stdio.h>

extern const unsigned int dimensions; //const - this is for the benifit of the header generator

short set_dimensions(unsigned int i) ;

typedef float* vector;

inline vector Vmk(void) ;

vector Vheap(const vector const a) ;


void Vzero(vector const a) ;


void Vadd(vector const a, const vector const b, const vector const c) ;

void Vsub(vector const a, const vector const b, const vector const c) ;


void Vtimes_scalar(vector const a, const vector const b, const float c) ;

float Vdot(const vector const a, const vector const b) ;

float Vmodulus(const vector const a) ;

/* Useful for testing components
void Vprint(const vector const a) ;
*/

#endif
