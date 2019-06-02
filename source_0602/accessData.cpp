
#include <pthread.h>
#include "accessData.h"


void set(int *, int, pthread_mutex_t *);
int get(int *, pthread_mutex_t *);

void setmsg(int *msg, int pmsg, pthread_mutex_t *mutex)
{
	set(msg, pmsg, mutex);
}

void setGesType(int *GesType, int pGesType, pthread_mutex_t *mutex)
{
	set(GesType, pGesType, mutex);
}

void setsw(int *sw, int psw, pthread_mutex_t *mutex)
{
	set(sw, psw, mutex);
}

int getmsg(int *msg, pthread_mutex_t *mutex)
{
	return get(msg, mutex);
}

int getGesType(int *GesType, pthread_mutex_t *mutex)
{
	return get(GesType, mutex);
}

int getsw(int *sw, pthread_mutex_t *mutex)
{
	return get(sw, mutex);
}

void set(int *r, int s, pthread_mutex_t *mutex)
{
	pthread_mutex_lock(mutex);
	*r = s;
	pthread_mutex_unlock(mutex);
}

int get(int *r, pthread_mutex_t *mutex)
{
	pthread_mutex_lock(mutex);
	int ret = *r;
	pthread_mutex_unlock(mutex);

	return ret; 
}


