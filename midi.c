#include "io.h"

typedef short (*handler_t)(unsigned char*, size_t, void*);

static handler_t *handlers = NULL;
static void **handler_data = NULL;
static unsigned short n_handlers = 0;

static short midi_handler(unsigned char *event, size_t size) {
	unsigned short n;
	for (n=0; n<n_handlers; n++)
		if ( (*(handlers[n]))(event, size, handler_data[n]) != 0)
			return 1;
	return 0;
}

void register_handler(handler_t handler, void *data) {
	//add a handler and its data to the arrays
	void (*fptr)(short (*)(unsigned char*, size_t));
	unsigned short n;
	handler_t *h = calloc(n_handlers+1, sizeof(handler_t));
	void **hd = calloc(n_handlers+1, sizeof(void**));
	if (n_handlers == 0) { //create the midi port if it doesn't exist
		fptr = io_func("create_midi_in");
		(*fptr)(&midi_handler);
	}
	for (n=0; n<n_handlers; n++) {
		h[n] = handlers[n];
		hd[n] = handler_data[n];
	}
	free(handlers);
	free(handler_data);
	h[n] = handler;
	hd[n] = data;
	handlers = h;
	handler_data = hd;
	n_handlers++;
}
