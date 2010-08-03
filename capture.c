#include <dlfcn.h>
#include <stdlib.h>
#include <stdio.h>
#include "tempmodel.h"
#include "ptrlist.h"
#include "capture/plugin_header.h"

/*
 * Provides dynamic loading of capture plugins.
 * Capture plugins are for things like mics and pickups.
 */

typedef struct cdil {
	line_list *ll;
	char * device;
	struct cdil *next;
} cdil; //capture device initialization list

static cdil *devices = NULL;
static unsigned short no_devices = 0;
static capture_device **capdevs = NULL;

void register_capture(char *device, line_list *head) {
	cdil *new;
	if (devices == NULL) {
		new = devices = malloc(sizeof(cdil));
	}
	else {
		new = devices;
		while (new->next != NULL) {
			new = new->next;
		}
		new->next = malloc(sizeof(cdil));
		new = new->next;
	}
	new->ll = head;
	new->device = device;
	new->next = NULL;
	no_devices++;
}

short init_capture(temp_point *tree) {
	unsigned short i;
	char *str;
	void *libhandle;
	cdil *nxtdev;
	void *(*fptr)(line_list*, temp_point*);
	capdevs = calloc(no_devices, sizeof(capture_device*));
	for (i=0; i<no_devices; i++) {
		//create the path string for the capture device file
		str = calloc(strlen(devices->device)+11, sizeof(char));
		strcpy(str, "capture/");
		strcat(str, devices->device);
		strcat(str, ".so");
		//load the capture device
		libhandle = dlopen(str, RTLD_NOW); //might be memory inefficient, but guarantees that no loading can be required during rt operation
		free(str);
		if (!libhandle) {
			printf("error in dlopen(%s)\n%s\n", "point_velocity.so", dlerror());
			return 1;
		}
		fptr = dlsym(libhandle, "init");
		capdevs[i] = (*fptr)(devices->ll, tree);
		//free memory for the initializer and move on to the next one
		llfree(devices->ll);
		free(devices->device);
		nxtdev = devices->next;
		free(devices);
		devices = nxtdev;
	}
	return 0;
}
