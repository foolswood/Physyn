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

static cdil *devices = NULL;
static unsigned short no_devices = 0;
static capture_device **capdevs = NULL;

void register_capture(char *device, char *portname, line_list *head) {
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
	new->portname = portname;
	new->next = NULL;
	no_devices++;
}

short (*push_audio)(float*, const unsigned short);

void get_output(void) {
	unsigned short device_no;
	float output;
	for (device_no=0; device_no<no_devices; device_no++) { //this loop is a prime candidate for parallelization
		output = (capdevs[device_no]->get_output)((void*) capdevs[device_no]->data);
		(*push_audio)(&output, capdevs[device_no]->out_id); //do something on failure
	}
}

short init_capture(temp_point *tree) {
	unsigned short i;
	char *str;
	void *libhandle;
	cdil *nxtdev;
	void *(*fptr)(line_list*, temp_point*);
	unsigned short (*create_audio_out)(char*) = io_func("create_audio_out");
	push_audio = io_func("push_audio");
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
		capdevs[i]->data = (*fptr)(devices->ll, tree);
		//get other functions
		capdevs[i]->get_output = dlsym(libhandle, "get_out");
		capdevs[i]->free_data = dlsym(libhandle, "free_data");
		//create audio output
		capdevs[i]->out_id = (*create_audio_out)(devices->portname);
		//free memory for the initializer and move on to the next one
		llfree(devices->ll);
		free(devices->device);
		free(devices->portname);
		nxtdev = devices->next;
		free(devices);
		devices = nxtdev;
	}
	return 0;
}

void free_capture(void) {
	unsigned short device_no;
	for (device_no=0; device_no<no_devices; device_no++) {
		(capdevs[device_no]->free_data)((void*) capdevs[device_no]->data);
		free(capdevs[device_no]);
	}
	free(capdevs);
}
