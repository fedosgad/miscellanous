#include <pthread.h>

#include "misc.h"

void idle_threads_inc() {

	pthread_mutex_lock(&idle_threads_mutex);
	idle_threads += 1;
	pthread_mutex_unlock(&idle_threads_mutex);

	return;
}

void idle_threads_dec() {

	pthread_mutex_lock(&idle_threads_mutex);
	idle_threads -= 1;
	pthread_mutex_unlock(&idle_threads_mutex);

	return;
}
