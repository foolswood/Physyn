#include <dlfcn.h>
#include <stdlib.h>
#include <string.h>
#include "tempmodel.h"

static void *libhandle = NULL;

unsigned int load_io(const char const *name) {
	char *str;
	unsigned int (*fptr)(const char const*);
	//IO module file path
	str = calloc(strlen(name)+6, sizeof(char));
	strcpy(str, "io/");
	strcat(str, name);
	strcat(str, ".so");
	//load module
	libhandle = dlopen(str, RTLD_NOW);
	if (!libhandle) {
		printf("error in dlopen(%s)\n%s\n", str, dlerror());
		free(str);
		return 0;
	}
	free(str);
	//call io setup
	fptr = dlsym(libhandle, "init");
	return fptr(name);
}

//this doesn't add much
void *io_func(const char const *str) {
	return dlsym(libhandle, str);
}
