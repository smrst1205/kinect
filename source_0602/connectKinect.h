
void *connectKinect(void *);

typedef struct {
	int *sw;
	pthread_mutex_t *mutex;
	int *GesType;
} kinectarg_t;


