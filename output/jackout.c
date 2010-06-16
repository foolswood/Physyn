#include <jack.h>

unsigned int init_output(llist *head); //returns sample rate

//some kind of callback acceptable to jack to collect from a ringbuffer (can use jack ringbuffer)
