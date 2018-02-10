#include <stdio.h>
#include <time.h>

#include "debug.h"

void log_action(const char* text) {

	printf("%s\n", text);
	fprintf(client_log_file, "%ld ", (long int)time(NULL));		//timestamp
	fprintf(client_log_file, "%s\n", text);

	return;

}


void log_action_num(const char* text, long int num) {

	printf("%s %ld\n", text, num);
	fprintf(client_log_file, "%ld ", (long int)time(NULL));		//timestamp
	fprintf(client_log_file, "%s %ld\n", text, num);

	return;

}
