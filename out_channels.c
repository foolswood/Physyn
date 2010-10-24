#include <stdlib.h>
#include <jack/jack.h>
#include <jack/ringbuffer.h>

static jack_client_t *client;

unsigned int out_init(void) {
	//creates the jack client and returns the sample rate (0 is error)
	client = jack_client_open("physyn", JackNoStartServer, NULL);
	if (client == NULL)
		return 0;
	return (unsigned int) jack_get_sample_rate(client);
}

typedef struct outport {
	jack_port_t *port;
	jack_ringbuffer_t *rb;
	struct outport *next;
} outport;

static outport *op = NULL;

jack_ringbuffer_t *create_output(char *portname) {
	jack_port_t *port = jack_port_register(client, portname, JACK_DEFAULT_AUDIO_TYPE, JackPortIsOutput, 0);
	jack_ringbuffer_t *rb = jack_ringbuffer_create(4096*sizeof(float));
	jack_ringbuffer_mlock(rb);
	outport *o = malloc(sizeof(outport));
	o->port = port;
	o->rb = rb;
	o->next = op;
	op = o;
	return rb;
}

static int process(jack_nframes_t nframes, void *voidptr) {
	static char *out;
	static outport *o;
	o = (outport*) voidptr;
	while (o != NULL) {
		out = (char *) jack_port_get_buffer(o->port, nframes);
		jack_ringbuffer_read(o->rb, out, sizeof(float)*nframes);
		o = o->next;
	}
	return 0;
}

void jack_go(void) {
	jack_set_process_callback(client, process, (void*) op);
	jack_activate(client);
	jack_connect(client, "physyn:out_1", "system:playback_1");
}
