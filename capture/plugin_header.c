#include <jack/ringbuffer.h>

/*
point *get_point(char *id);
point **get_all_points(void);
*/

typedef struct capture_device {
	float (*get_output) (void*);
	void *data;
	jack_ringbuffer_t *ringbuf;
} capture_device;
