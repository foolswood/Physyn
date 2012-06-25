#ifndef MIDI_HELPERSHEADER
#define MIDI_HELPERSHEADER

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

unsigned short note_id(char *note_id) ;

#endif
