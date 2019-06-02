
#include <iostream>
#include <stdexcept>
#include <pthread.h>
#include <stdio.h>

#include "connectCamera.h"

#include "accessData.h"

void *connectCamera(void *p)
{

	cameraarg_t *arg = (cameraarg_t *)p;
	int msg = getsw(arg->msg, arg->mutex);
	int print = 0;

	// Main loop
	while (getsw(arg->sw, arg->mutex)) {
		msg = getsw(arg->msg, arg->mutex);
		switch(msg){
		case 0:
			break;
		case 1:
		printf("msg >> %d\n", msg);
			break;
		case 2:
		printf("msg >> %d\n", msg);
			break;
		case 3:
		printf("msg >> %d\n", msg);
			break;
		case 4:
		printf("msg >> %d\n", msg);
			break;
		default :
			;
		}
		*arg->msg = 0;
	}
}
