#include <stdio.h>	//general IO
#include <stdlib.h>	//malloc()
#include <unistd.h>
#include <pthread.h>	//threads
#include <error.h>	//error()
#include <errno.h>	//error codes
#include <time.c>	//timestamps


#define PORT 12345
#define MAX_THREADS 100



void * handle_connection(void* );



int main(int argc, char* argv[]) {

	int listenfd;
	struct sockaddr_in serv_addr;

	pthread_t *thread_id;





	return 0;
}
