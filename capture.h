#ifndef CAPTUREHEADER
#define CAPTUREHEADER

#include <dlfcn.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include "tempmodel.h"
#include "linelist.h"
#include "io.h"

/*
 * Provides dynamic loading of capture plugins.
 * Capture plugins are for things like mics and pickups.
 */

typedef struct capture_device {
	float (*get_output) (void*);
	unsigned short out_id;
	void *data;
	void (*free_data) (void*);
} capture_device;

typedef struct cdil {
	line_list *ll;
	char *device;
	char *portname;
	struct cdil *next;
} cdil; //capture device initialization list


void register_capture(char *device, char *portname, line_list *head) ;

short (*push_audio)(float*, const unsigned short);

void get_output(void) ;

short init_capture(temp_point *tree) ;

void free_capture(void) ;

#endif
