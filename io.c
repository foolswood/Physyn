#include <dlfcn.h>

static void *libhandle = NULL;

unsigned int load_io(const char const *name, temp_point *tree) {
	char *str;
	unsigned int (*fptr)(temp_point*);
	//IO module file path
	str = calloc(strlen(name)+6, sizeof(char));
	strcpy(str, "IO/");
	strcat(str, name);
	strcat(str, ".so");
	//load module
	libhandle = dlopen(str, RTLD_NOW);
	free(str);
	if (!libhandle) {
		printf("error in dlopen(%s)\n%s\n", str, dlerror());
		return 0;
	}
	//call action setup
	fptr = dlsym(libhandle, "init");
	return fptr(tree);
}

//this doesn't add much
void *io_func(const char const *str) {
	return dlsym(libhandle, str);
}
