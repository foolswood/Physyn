#include <stdlib.h>
#include <jack/jack.h>
#include <jack/ringbuffer.h>

static jack_client_t *client;

unsigned int jack_init(void) {
	//creates the jack client and returns the sample rate (0 is error)
	client = jack_client_open("physyn", JackNoStartServer, NULL);
	if (client == NULL)
		return 0;
	return (unsigned int) jack_get_sample_rate(client);
}

//Audio

typedef struct audioport {
	jack_port_t *port;
	jack_ringbuffer_t *rb;
	struct audioport *next;
} audioport;

enum audioport_type {
	AUDIOPORT_INPUT,
	AUDIOPORT_OUTPUT
};

static audioport *op = NULL;
static audioport *ip = NULL;

static unsigned short BUFFERSIZE = 4096; //Buffer size (samples), default

jack_ringbuffer_t *create_audioport(char *portname, int port_type) { //Inputs must be created first
	jack_port_t *port;
	jack_ringbuffer_t *rb = jack_ringbuffer_create(BUFFERSIZE*sizeof(float));
	audioport *p = malloc(sizeof(audioport));
	if ( port_type ) { //Output
		if ( ip == NULL ) //No Audio Input Ports
			port = jack_port_register(client, portname, JACK_DEFAULT_AUDIO_TYPE, JackPortIsOutput | JackPortIsTerminal, 0);
		else
			port = jack_port_register(client, portname, JACK_DEFAULT_AUDIO_TYPE, JackPortIsOutput, 0);
	} else { //Input
		port = jack_port_register(client, portname, JACK_DEFAULT_AUDIO_TYPE, JackPortIsInput, 0);
	}
	jack_ringbuffer_mlock(rb);
	p->port = port;
	p->rb = rb;
	if ( port_type ) {
		p->next = op;
		op = p;
	} else {
		p->next = ip;
		ip = p;
	}
	return rb;
}

//Midi

static unsigned short MAX_MIDI_EVENTS = 24; //Maximum Number Of Midi Events In A Frame To Accept

//Consider how to buffer midi events

//Jack

static int process(jack_nframes_t nframes, void *voidptr) {
	static char *out;
	static audioport *o;
	o = (audioport*) voidptr;
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
	jack_connect(client, "physyn:out_1", "system:playback_2");
}
