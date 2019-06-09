
class CameraConnector
{
	int sw;
	int Feedback;
	int Msg;
	pthread_mutex_t mutex;
public:

	CameraConnector();
	void *sendMessage();
	void setMsg(int);
	int getFeedback();
	void setsw(int);
	void setFeedback(int);
};


