#ifndef ACTION_QUEUEHEADER
#define ACTION_QUEUEHEADER

#include <stdlib.h>

typedef struct actionlist {
	unsigned short (*act) (void*); //return 0 until it is to be destroyed, then return 1
	void *data;
	void (*free_data) (void*);
	struct actionlist *next;
} actionlist;

typedef enum act_when {
	ACT_START,
	ACT_MIDDLE,
	ACT_END
}act_when;


void add_action(int when, unsigned short (*act) (void*), void *data, void (*free_data) (void*)) ;

void do_actions(int when) ;

#endif
