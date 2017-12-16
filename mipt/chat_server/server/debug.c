#include <stdio.h>
#include <time.h>

#include "debug.h"

void log_action(const char* text) {

	printf("%s\n", text);
	fprintf(server_log_file, "%ld ", (long int)time(NULL));		//timestamp
	fprintf(server_log_file, "%s\n", text);

	return;

}


void log_action_num(const char* text, int num) {

	printf("%s %i\n", text, num);
	fprintf(server_log_file, "%ld ", (long int)time(NULL));		//timestamp
	fprintf(server_log_file, "%s %i\n", text, num);

	return;

}
