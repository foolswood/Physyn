#ifndef MIDI_HELPERSHEADER
#define MIDI_HELPERSHEADER

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

unsigned short note_id(char *note_id) ;

#endif
