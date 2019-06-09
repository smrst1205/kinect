
class KinectConnector
{
	int sw;
	int GesType;
	pthread_mutex_t mutex;
public:
	KinectConnector();
	void *getGesture();
	void setGesType(int);
	int getGesType();
	int getsw();
};

