#include <stdlib.h>
#include <unistd.h>
#include <jack/jack.h>
#include <jack/ringbuffer.h>
#include <jack/midiport.h>

static jack_client_t *client;
jack_nframes_t framesize;

unsigned int init(const char const *name) {
	//Returns the sample rate
	client = jack_client_open(name, JackNoStartServer, NULL);
	if (client == NULL)
		return 0;
	return (unsigned int) jack_get_sample_rate(client);
}

static jack_port_t **output_ports = NULL;
static jack_ringbuffer_t **output_buffers = NULL;
static unsigned short n_outputs = 0;

unsigned short create_audio_out(const char const *output_name) {
	unsigned short n;
	jack_port_t *port = jack_port_register(client, output_name, JACK_DEFAULT_AUDIO_TYPE, JackPortIsOutput, 0); //Modify to adapt if terminal
	jack_port_t **ports = calloc(n_outputs + 1, sizeof(jack_port_t*));
	for (n = 0; n < n_outputs; n++)
		ports[n] = output_ports[n];
	ports[n] = port;
	free(output_ports);
	output_ports = ports;
	return n_outputs++;
}

static jack_port_t *midi_port = NULL;
static jack_midi_event_t *midi_buffer = NULL;
static unsigned short n_mevents = 0;
static short (*midi_handler)(unsigned char*, size_t) = NULL;

void create_midi_in(short (*mh)(unsigned char*, size_t)) {
	midi_port = jack_port_register(client, "in", JACK_DEFAULT_MIDI_TYPE, JackPortIsInput, 0);
	midi_handler = mh;
}

//void *create_audio_in(char *name) {
//}

short push_audio(const float const *sample, const unsigned short out_id) {
	const char *sc = (const char *) sample;
	if (output_buffers == NULL)
		return 1;
	while (jack_ringbuffer_write_space(output_buffers[out_id]) < (framesize*sizeof(float))-1) {
		usleep(500); //Do something cleverer (signals or locks something)
	}
	jack_ringbuffer_write(output_buffers[out_id], sc, sizeof(float));
	return 0;
}

unsigned int pm_called = 0;
short pull_midi(void) {
	static short rcode, n;
	static jack_midi_event_t *mb;
	pm_called++;
	for (n=0; n<n_mevents; n++) {
		mb = &(midi_buffer[n]);
		if (mb->time) {
			mb->time--;
			if (!mb->time) {
				rcode = midi_handler((unsigned char*) mb->buffer, mb->size);
				if (rcode)
					return rcode;
			}
		}
	}
	return 0;
}

//short pull_audio(void *voidptr) {
//}

short pull(void) {
	if (midi_port != NULL)
		if (pull_midi() == 0)
			return 0;
	return 1;
}

static int set_buffersize(jack_nframes_t nframes, void *voidptr) {
	//Creates ringbuffers of the appropriate length (currently clears them)
	unsigned short n;
	float zf = 0.0;
	framesize = nframes;
	if (output_buffers == NULL)
		output_buffers = calloc(n_outputs, sizeof(jack_ringbuffer_t*));
	else {
		for (n = 0; n < n_outputs; n++)
			jack_ringbuffer_free(output_buffers[n]);
	}
	for (n = 0; n < n_outputs; n++) {
		output_buffers[n] = jack_ringbuffer_create(nframes*sizeof(float)*2);
		jack_ringbuffer_mlock(output_buffers[n]);
		//initially fill with zeros
		while (jack_ringbuffer_write_space(output_buffers[n]) >= sizeof(float))
			jack_ringbuffer_write(output_buffers[n], (char*) &zf, sizeof(float));
	}
	//MIDI?
	midi_buffer = calloc(4, sizeof(jack_midi_event_t)); //currently allow 4 events per frame, when the jack function comes into debian I will update
	return 0;
}

static int process(jack_nframes_t nframes, void *voidptr) {
	static unsigned short n;
	static jack_midi_event_t *mb;
	//Jack Process Callback
	//Midi
	pm_called = 0;
	n_mevents = 0;
	mb = midi_buffer;
	while (jack_midi_event_get(mb, jack_port_get_buffer(midi_port, nframes), n_mevents) == 0) {
		mb->time++;
		mb = &(midi_buffer[n]);
		n_mevents++;
		if (n_mevents == 4)
			break;
	}
	//Audio
	for (n = 0; n < n_outputs; n++) {
		if (jack_ringbuffer_read_space(output_buffers[n]) < nframes*sizeof(float))
			return 1;
		jack_ringbuffer_read(output_buffers[n], jack_port_get_buffer(output_ports[n], nframes), nframes*sizeof(float));
	}
	return 0;
}

void go(void) {
	set_buffersize(jack_get_buffer_size(client), NULL);
	//do a jack_latency callback
	jack_set_buffer_size_callback(client, set_buffersize, NULL);
	jack_set_process_callback(client, process, NULL);
	jack_activate(client);
	//autoconnect ports
	jack_connect(client, "physyn:out_1", "system:playback_1");
	jack_connect(client, "physyn:out_1", "system:playback_2");
}
