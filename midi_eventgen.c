#include <jack/jack.h>
#include <jack/midiport.h>
#include <unistd.h>

//typedef a struct of jack port and boolean
typedef struct jport {
	jack_port_t *port;
	short going;
} jport;

int gen_midi_events(jack_nframes_t nframes, void *voidptr) { //create an event when the boolean = true
	jport *pt = voidptr;
	void *jbuffer = jack_port_get_buffer((*pt).port, nframes);
	unsigned char* buffer;
	if ( (*pt).going ) {
		jack_midi_clear_buffer(jbuffer);
		buffer = jack_midi_event_reserve(jbuffer, 0, 3);
		buffer[2] = 64; //velocity
		buffer[1] = 60; //note
		buffer[0] = 0x90; //note on
		(*pt).going = 0;
	}
	return 0;
}

int main(void) {
	jack_client_t *client = jack_client_open("eventgen", JackNoStartServer, NULL);
	jack_port_t *port = jack_port_register(client, "eventout", JACK_DEFAULT_MIDI_TYPE, JackPortIsOutput, 0);
	jport pt = {port, 0};
	jack_set_process_callback(client, gen_midi_events, (void*) &pt);
	jack_activate(client);
	while (1) {
		pt.going = 1;
		sleep(2);
	}
	return 0;
}
