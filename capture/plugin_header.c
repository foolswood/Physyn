/*
point *get_point(char *id);
point **get_all_points(void);
*/

typedef struct capture_device {
	float (*get_output) (void*);
	void *data;
} capture_device;
