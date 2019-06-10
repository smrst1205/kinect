
#ifndef __POINT__
#define __POINT__
typedef struct {
	float x;
	float y;
	float z;
} point_t;
#endif
class KinectConnector
{
	int sw;
	int GesType;
	point_t point;
	pthread_mutex_t mutex;
public:
	KinectConnector();
	void *getGesture();
	void setGesType(int);
	int getGesType();
	int getsw();
	void setPoint(point_t);
	point_t getPoint();
};

