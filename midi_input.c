#include <jack/jack.h>
#include <jack/midiport.h>
#include <stdio.h>
#include <unistd.h>

int print_on_midi_events(jack_nframes_t nframes, void *voidptr) {
	jack_port_t *port = voidptr;
	jack_midi_event_t *event;
	if ( jack_midi_event_get(event, jack_port_get_buffer(port, nframes), 0) == 0 ) {
		printf("event\n");
	}
	return 0;
}

int main(void) {
	jack_client_t *client = jack_client_open("testing", JackNoStartServer, NULL);
	jack_port_t *port = jack_port_register(client, "midiprinter", JACK_DEFAULT_MIDI_TYPE, JackPortIsInput, 0);
	jack_set_process_callback(client, print_on_midi_events, (void*) port);
	jack_activate(client);
	while (1) {
		sleep(1);
	}
	return 0;
}
