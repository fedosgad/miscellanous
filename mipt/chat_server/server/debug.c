#include <stdio.h>
#include <time.h>

#include "debug.h"

void log_action(const char* text) {

	printf("%s\n", text);
	fprintf(server_log_file, "%ld ", (long int)time(NULL));		//timestamp
	fprintf(server_log_file, "%s\n", text);

	return;

}


void log_action_num(const char* text, long int num) {

	printf("%s %ld\n", text, num);
	fprintf(server_log_file, "%ld ", (long int)time(NULL));		//timestamp
	fprintf(server_log_file, "%s %ld\n", text, num);

	return;

}