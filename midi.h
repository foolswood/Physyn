#ifndef MIDIHEADER
#define MIDIHEADER

#include "io.h"

typedef short (*handler_t)(unsigned char*, size_t, void*);



void register_handler(handler_t handler, void *data) ;

#endif
