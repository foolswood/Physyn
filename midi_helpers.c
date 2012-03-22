#include <stdlib.h>

#define NOTE_ON_EVENT 0x90
#define NOTE_OFF_EVENT 0x80
#define NOTE_AFTERTOUCH_EVENT 0xA0
#define CONTROLLER_EVENT 0xB0
#define PROGRAM_CHANGE_EVENT 0xC0
#define CHANNEL_AFTERTOUCH_EVENT 0xD0
#define PITCH_BEND_EVENT 0xE0

typedef struct midi_channel_event_t {
	unsigned type:4;
	unsigned channel:4;
	unsigned arg1:8;
	unsigned arg2:8;
} midi_channel_event_t;

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
