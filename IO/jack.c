#include <stdlib.h>
#include <unistd.h>
#include <jack/jack.h>
#include <jack/ringbuffer.h>

static jack_client_t *client;

unsigned int init(const char const *name) {
	//Returns the sample rate
	client = jack_client_open(name, JackNoStartServer, NULL);
	if (client == NULL)
		return 0;
	return (unsigned int) jack_get_sample_rate(client);
}

static jack_port_t *output_ports = NULL;
static jack_ringbuffer_t *output_buffers = NULL;
static unsigned short n_outputs = 0;

unsigned short create_audio_out(const char const *output_name) {
	jack_port_t *port = jack_port_register(client, output_name, JACK_DEFAULT_AUDIO_TYPE, JackPortIsOutput, 0); //Modify to adapt if terminal
	jack_port_t *ports = calloc(n_outputs + 1, sizeof(jack_port_t**));
	for (n = 0; n < n_outputs; n++)
		ports[n] = output_ports[n];
	ports[n_outputs] = port;
	return ++n_outputs;
}

//void *create_midi_in(char *name) {
//}

//void *create_audio_in(char *name) {
//}

short push_audio(const float const *sample, const unsigned short out_id) {
	const char *sc = (const char *) sample;
	while (jack_ringbuffer_write_space(output_buffers[out_id]) < sizeof(float))
		usleep(100); //Do something cleverer (signals or locks something)
	jack_ringbuffer_write(output_buffers[out_id], sc, sizeof(float));
}

//short pull_midi(void *voidptr) {
//}

//short pull_audio(void *voidptr) {
//}

static int set_buffersize(jack_nframes_t nframes, void *voidptr) {
	//Creates ringbuffers of the appropriate length (currently clears them)
	if (output_buffers == NULL)
		output_buffers = calloc(n_outputs, sizeof(jack_ringbuffer_t*));
	else {
		for (n = 0; n < n_outputs; n++)
			jack_ringbuffer_free(output_buffers[n]);
	}
	for (n = 0; n < n_outputs; n++) {
		output_ports[n] = jack_ringbuffer_create(nframes*sizeof(float));
		jack_ringbuffer_mlock(output_ports[n]);
	}
}

static int process(jack_nframes_t nframes, void *voidptr) {
	//Jack Process Callback
	for (n = 0; n < n_outputs; n++) {
		if (jack_ringbuffer_read_space(output_buffers[n]) < nframes)
			return 1;
		jack_ringbuffer_read(output_buffers[n], jack_port_get_buffer(output_ports[n], nframes), nframes);
	}
	return 0;
}

void go(void) {
	//do a jack_latency callback
	jack_set_buffersize_callback(client, set_buffersize, NULL);
	jack_set_process_callback(client, process, NULL);
	jack_activate(client);
	//autoconnect ports
	jack_connect(client, "physyn:out_1", "system:playback_1");
	jack_connect(client, "physyn:out_1", "system:playback_2");
}
