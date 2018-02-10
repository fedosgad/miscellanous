#include <stdio.h>
#include <time.h>
#include <pthread.h>

#include "debug.h"

void log_action(const char* text) {

	printf("%s\n", text);
	pthread_mutex_lock(&server_log_file_mutex);
	fprintf(server_log_file, "%ld ", (long int)time(NULL));		//timestamp
	fprintf(server_log_file, "%s\n", text);
	pthread_mutex_unlock(&server_log_file_mutex);

	return;

}


void log_action_num(const char* text, long int num) {

	printf("%s %ld\n", text, num);
	pthread_mutex_lock(&server_log_file_mutex);
	fprintf(server_log_file, "%ld ", (long int)time(NULL));		//timestamp
	fprintf(server_log_file, "%s %ld\n", text, num);
	pthread_mutex_unlock(&server_log_file_mutex);

	return;

}
