#include <stdlib.h>

//Event and channel bitmasks
#define MIDI_EVENT 0xF0
#define MIDI_CHANNEL 0x0F

//Event types
#define MIDI_NOTE_ON 0x90
#define MIDI_NOTE_OFF 0x80
#define MIDI_NOTE_AFTERTOUCH 0xA0
#define MIDI_CONTROLLER 0xB0
#define MIDI_PROGRAM_CHANGE 0xC0
#define MIDI_CHANNEL_AFTERTOUCH 0xD0
#define MIDI_PITCH_BEND 0xE0

unsigned short note_id(char *note_id) {
	//Bulletproof me
	unsigned short id, octave, offset;
	switch (*note_id) {
		case 'C':
			id = 0;
			break;
		case 'D':
			id = 2;
			break;
		case 'E':
			id = 4;
			break;
		case 'F':
			id = 5;
			break;
		case 'G':
			id = 7;
			break;
		case 'A':
			id = 9;
			break;
		case 'B':
			id = 11;
			break;
		default:
			id = 9; //Default to A
			break;
	}
	switch (*(note_id+sizeof(char))) {
		case '#':
			id++;
			offset = 2;
			break;
		case 'b':
			id--;
			offset = 2;
			break;
		default:
			offset = 1;
			break;
	}
	octave = atoi(note_id + (offset*sizeof(char)));
	return id + (12*octave);
}
