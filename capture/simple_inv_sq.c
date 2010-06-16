// A simple inverse square mic

typedef struct mic {
	vector loc; //mic location
	float scale; //multiplier to prevent clipping
} mic;

mic create_mic(llist *head); //read a section of the config file to get the data to initialize the mic

float mic_out(mic m, pointset *p); //get the output from the microphone
