void manageApp(void);

class AppController
{
private:
    int sw;
	int GesType;
    int Feedback;
	int Msg;
    pthread_mutex_t mutex;
public:
    AppController();
    void manageApp();
};

