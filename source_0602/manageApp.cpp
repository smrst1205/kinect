
#include <iostream>
#include <stdexcept>
#include <pthread.h>
#include <stdio.h>

#include "manageApp.h"
#include "connectKinect.h"
#include "connectCamera.h"

#include "accessData.h"

void manageApp()
{


	pthread_t thread[2];
	int sw = 1;
	pthread_mutex_t mutex;
	pthread_mutex_init(&mutex, NULL);
	int GesType = 0;
	int msg = 0;

	int pGesType = 0;
	int pmsg = 0;

	kinectarg_t kinectarg = {&sw, &mutex, &GesType};
	cameraarg_t cameraarg = {&sw, &mutex, &msg};



	pthread_create(&thread[0], NULL, (void* (*)(void*))connectKinect, (void *)(&kinectarg));
	pthread_create(&thread[1], NULL, (void* (*)(void*))connectCamera, (void *)(&cameraarg));



	// Main loop
	while (getsw(&sw, &mutex)) {
		if(getGesType(&GesType, &mutex) != 0){
			//printf("GesType>%d\n",getGesType(&GesType, &mutex));
			switch(GesType){
			case 0:
				msg = 0;
				break;
			case 1:
				msg = 1;
				break;
			case 2:
				msg = 2;
				break;
			case 3:
				msg = 3;
				break;
			case 4:
				msg = 4;
				break;
			default:
				msg = 0;
			}
			setGesType(&GesType, 0, &mutex);
			pGesType = 0;
		} 
	}
	pthread_join(thread[0], NULL);
	pthread_join(thread[1], NULL);
}


