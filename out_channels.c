typedef struct channel {
	ringbuffer rb;
	char *id;
	channel *next;
} channel;

#include "output.c"
#include "capture.c"


