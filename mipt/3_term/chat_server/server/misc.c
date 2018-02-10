#include <stdio.h>
#include <pthread.h>
#include <error.h>
#include <errno.h>
#include <sys/types.h>

#include "misc.h"
#include "debug.h"


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

void create_new_thread(pthread_t* id, void *(*funct) (void *)) {

	int flag = 0, status = 0;

	while(!flag) {
		status = pthread_create(id, (pthread_attr_t*)NULL, funct, (void*)NULL);
		if(status != 0) {
			log_action("Failed to create new thread");
			error(0, status, "Failed to create new thread");
		}
		else
			flag = 1;
	}
	flag = 0;
	log_action_num("Created new thread, id", (ulong)*id);


	return;
}
