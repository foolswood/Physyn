#include <dlfcn.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <jack/ringbuffer.h>
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
	jack_ringbuffer_t *ringbuf;
	struct cdil *next;
} cdil; //capture device initialization list

static cdil *devices = NULL;
static unsigned short no_devices = 0;
static capture_device **capdevs = NULL;

unsigned short register_capture(char *device, jack_ringbuffer_t *ringbuf, line_list *head) {
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
	new->ringbuf = ringbuf;
	new->next = NULL;
	return no_devices++;
}

void get_output(void) {
	unsigned short device_no;
	float output;
	for (device_no=0; device_no<no_devices; device_no++) { //this loop is a prime candidate for parallelization
		output = (capdevs[device_no]->get_output)((void*) capdevs[device_no]->data);
		while (jack_ringbuffer_write_space(capdevs[device_no]->ringbuf) < sizeof(float)) {
			usleep(200);
		}
		jack_ringbuffer_write(capdevs[device_no]->ringbuf, (char*) &output, sizeof(float));
	}
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
			printf("error in dlopen(%s)\n%s\n", str, dlerror());
			return 1;
		}
		//call the device's init function
		fptr = dlsym(libhandle, "init");
		capdevs[i] = (*fptr)(devices->ll, tree);
		capdevs[i]->ringbuf = devices->ringbuf;
		//free memory for the initializer and move on to the next one
		llfree(devices->ll);
		free(devices->device);
		nxtdev = devices->next;
		free(devices);
		devices = nxtdev;
	}
	return 0;
}
