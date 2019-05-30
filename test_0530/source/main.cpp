
#include <iostream>
#include <stdexcept>
#include <pthread.h>
#include <stdio.h>
#include "kinectconnector.h"


pthread_mutex_t mutex;


int main(int argc, char** argv)
{
	int pGesType = 0;
	pthread_t	thread;
	kinectarg_t	kinearg;
	kinearg.sw = 1;
	kinearg.GesType = 0;

	pthread_mutex_init(&mutex, NULL);
	pthread_create(&thread, NULL, (void* (*)(void*))kinectconnector, (void *)(&kinearg));



	// Main loop
	while (kinearg.sw) {
		if(pGesType!=kinearg.GesType){
			printf("GesType>%d\n",kinearg.GesType);
			pthread_mutex_lock(&mutex);
			pGesType=kinearg.GesType;
			kinearg.GesType = 0;
			pthread_mutex_unlock(&mutex);
		}
	}
	pthread_join(thread, NULL);

	return 0;
}