
void *kinectconnector(void *);

typedef struct {
	int sw;
	int GestureType;
} kinectarg_t;

extern pthread_mutex_t mutex;