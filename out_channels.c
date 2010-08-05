#include <stdlib.h>
#include <jack/jack.h>
#include <jack/ringbuffer.h>

static jack_client_t *client;

short out_init(void) {
	client = jack_client_open("physyn", JackNoStartServer, NULL);
	return 0;
}

typedef struct outport {
	jack_port_t *port;
	jack_ringbuffer_t *rb;
	struct outport *next;
} outport;

static outport *op = NULL;

jack_ringbuffer_t *create_output(char *portname) {
	jack_port_t *port = jack_port_register(client, portname, JACK_DEFAULT_AUDIO_TYPE, JackPortIsOutput & JackPortIsTerminal, 2*sizeof(JACK_DEFAULT_AUDIO_TYPE));
	jack_ringbuffer_t *rb = jack_ringbuffer_create(1024*sizeof(float));
	jack_ringbuffer_mlock(rb);
	outport *o = malloc(sizeof(outport));
	o->port = port;
	o->rb = rb;
	o->next = op;
	op = o;
	return rb;
}

int process(jack_nframes_t nframes, void *voidptr) {
	char *out;
	outport *o = voidptr;
	while (o != NULL) {
		out = (char *) jack_port_get_buffer(o->port, nframes);
		jack_ringbuffer_read(o->rb, out, sizeof(float)*nframes);
		o = o->next;
	}
	return 0;
}
