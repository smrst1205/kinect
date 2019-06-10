#ifndef __POINT__
#define __POINT__
typedef struct {
	float x;
	float y;
	float z;
} point_t;
#endif
class AppController
{
private:
    int sw;
	int GesType;
    int Feedback;
	int Msg;
	point_t point;
    pthread_mutex_t mutex;
public:
    AppController();
    void manageApp();
};

