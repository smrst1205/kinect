
#include <iostream>
#include <stdexcept>
#include <pthread.h>

#include "manageApp.h"
#include "connectKinect.h"
#include "connectCamera.h"

AppController::AppController()
{
	sw = 1;
	GesType = 0;
	Feedback = 0;
	Msg = 0;
	pthread_mutex_init(&mutex, NULL);
}

void AppController::manageApp()
{
	KinectConnector kinect;
        CameraConnector camera;

        pthread_t thread[2]; 
	pthread_create(&thread[0], NULL, (void* (*)(void*))&KinectConnector::getGesture, (void *)&kinect);
	pthread_create(&thread[1], NULL, (void* (*)(void*))&CameraConnector::sendMessage, (void *)&camera);


	// Main loop
	while (sw = kinect.getsw()) {

		GesType  = kinect.getGesType();
		Feedback = camera.getFeedback();

		/*MsgÇÃê›íËã@ç\*/
		if(GesType!=0) {
		    Msg = GesType;
		    camera.setFeedback(1);
		}
		if(Feedback == 0) {
			GesType = 0;
			Msg = 0;
		}
		camera.setMsg(Msg);
        
 	}
	camera.setsw(0);
	pthread_join(thread[0], NULL);
	pthread_join(thread[1], NULL);

	std::cout << "App End" << std::endl;
}



