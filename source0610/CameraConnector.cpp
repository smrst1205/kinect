
#include <iostream>
#include <stdexcept>
#include <pthread.h>

#include "CameraController.h"

CameraConnector::CameraConnector()
{
	sw = 1;
	Feedback = 0;
	Msg = 0;
	pthread_mutex_init(&mutex, NULL);
}

void *CameraConnector::sendMessage()
{
	int i = 0;
	int x = 0;
	int pmsg = 0;
	int msg =0;
	while(sw) {
		msg = Msg;
		if(pmsg == msg) i=0;
		switch(msg){
		case 0:
			if(pmsg != 0)std::cout << "*******" << std::endl;
			x = 0;
			break;
		case 1:
			x = 1;
			if(pmsg != 1)std::cout << "RecON" << std::endl;
			Feedback = 0;
			break;
		case 2:
			x = 1;
			if(pmsg != 2)std::cout << "RecOFF" << std::endl;
			Feedback = 0;
			break;
		case 3:
			x = 1;
			if(pmsg != 3)std::cout << "ZoomIn" << std::endl;
			i++;
			if(i>100)	Feedback = 0;
			break;
		case 4:
			x = 1;
			if(pmsg != 4)std::cout << "ZoomOut" << std::endl;
			i++;
			if(i>100)	Feedback = 0;
			break;
		default :
			;
		}
		pmsg = msg;	
	}
}


void CameraConnector::setMsg(int type)
{
	pthread_mutex_lock(&mutex);	
	Msg = type;
	pthread_mutex_unlock(&mutex);
}

int CameraConnector::getFeedback()
{
	pthread_mutex_lock(&mutex);
	int ret = Feedback;
	pthread_mutex_unlock(&mutex);
	return ret; 
}

void CameraConnector::setFeedback(int type)
{
	pthread_mutex_lock(&mutex);	
	Feedback = type;
	pthread_mutex_unlock(&mutex);
}

void CameraConnector::setsw(int type)
{
	pthread_mutex_lock(&mutex);	
	sw = type;
	pthread_mutex_unlock(&mutex);
}


