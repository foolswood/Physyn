#ifndef IOHEADER
#define IOHEADER

#include <dlfcn.h>
#include <stdlib.h>
#include <string.h>
#include "tempmodel.h"


unsigned int load_io(const char const *name) ;

//this doesn't add much
void *io_func(const char const *str) ;

#endif
