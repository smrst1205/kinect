/*******************************************************************************
*                                                                              *
*   PrimeSense NITE 1.3 - Single Control Sample                                *
*   Copyright (C) 2010 PrimeSense Ltd.                                         *
*                                                                              *
*******************************************************************************/

//-----------------------------------------------------------------------------
// Headers
//-----------------------------------------------------------------------------
// General headers
#include <iostream>
#include <stdexcept>
#include <pthread.h>
#include <stdio.h>
#include "kinectconnector.h"


pthread_mutex_t mutex;


//-----------------------------------------------------------------------------
// Main
//-----------------------------------------------------------------------------


// this sample can run either as a regular sample, or as a client for multi-process (remote mode)
int main(int argc, char** argv)
{

	pthread_t thread;
	pthread_create(&thread, NULL, (void* (*)(void*))kinectconnector, (void *)NULL);
	pthread_mutex_init(&mutex, NULL);
	
	int p=0;

	printf("GestureType>%d\n",Gesture_Type);
	// Main loop
	while (sw) {
		if(p!=Gesture_Type){
			printf("GestureType>%d\n",Gesture_Type);
			pthread_mutex_lock(&mutex);
			p=Gesture_Type;
			Gesture_Type = 0;
			pthread_mutex_unlock(&mutex);
		}
	}
	pthread_join(thread, NULL);

	return 0;
}
