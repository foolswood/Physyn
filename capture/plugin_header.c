#include <jack/ringbuffer.h>

/*
point *get_point(char *id);
point **get_all_points(void);
*/

typedef struct capture_device {
	float (*get_output) (void*);
	jack_ringbuffer_t *ringbuf;
	void *data;
	void (*free_data) (void*);
} capture_device;
