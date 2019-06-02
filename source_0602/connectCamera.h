
void *connectCamera(void *);

typedef struct {
	int *sw;
	pthread_mutex_t *mutex;
	int *msg;
} cameraarg_t;


