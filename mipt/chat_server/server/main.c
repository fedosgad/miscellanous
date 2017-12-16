#include <stdio.h>	//general IO
#include <stdlib.h>	//malloc()
#include <unistd.h>	//close()
#include <pthread.h>	//threads
#include <error.h>	//error()
#include <errno.h>	//error codes
#include <time.h>	//timestamps
#include <sys/types.h>	//because required
#include <sys/socket.h>	//socket functions
#include <arpa/inet.h>	//htonl() and struct sockaddr_in


#include "main.h"
#include "client_interaction.h"
#include "debug.h"

int main(int argc, char* argv[]) {

	struct sockaddr_in serv_addr;
	FILE* tmp_chat_log;
	pthread_t *thread_id;

	int total_threads = 0, status, flag = 0;

	tmp_chat_log = init();


	thread_id = (pthread_t*)malloc(sizeof(pthread_t));
	while(!flag) {
		status = pthread_create(&(thread_id[0]), (pthread_attr_t*)NULL, handle_connection, (void*)NULL);
		if(status != 0) {
			log_action("Failed to create first thread");
			error(0, status, "Failed to create first thread");
		}
		else
			flag = 1;
	}
	flag = 0;
	log_action_num("Created first thread, id", (int)thread_id[0]);
	total_threads = 1;


	


	pthread_join(thread_id[0], (void**)NULL);
	free(thread_id);
	//~ deinit();


	return 0;
}


FILE* init() {

	int tries = 0, flag = 0, status;
	struct sockaddr_in serv_addr;
	FILE* tmp_chat_log;

/* Whatever we try to do here, we try to do it MAX_TRIES times 
 * We log every step if we can (if we have log file)
 */

	while((tries < MAX_TRIES) && !flag ) {		//log file itself
		printf("Trying to open log file\n");
		server_log_file = fopen(SERVER_LOG_FILE, "a");
		server_log_file != NULL ? flag = 1 : tries++;
	}
	if (!flag) 
		error(1, errno, "Can't open or create log file");
	flag = 0;
	tries = 0;
	setlinebuf(server_log_file);		//if we don't, we'll never see our logs
	log_action("Server started");


	while((tries < MAX_TRIES) && !flag ) {		//tmp log file for chat messages
		log_action("Trying to open temporary chat log");
		tmp_chat_log = tmpfile();
		tmp_chat_log != NULL ? flag = 1 : tries++;
	}
	if (!flag) {
		log_action("Can't open temporary chat log");
		error(1, errno, "Can't open temporary chat log");
	}
	else {
		log_action("Temporary chat log file opened");
		flag = 0;
		tries = 0;
	}


	while((tries < MAX_TRIES) && !flag ) {		//passwords file
		log_action("Try to open or create passwords file");
		passwd_file = fopen(PASSWD_FILE, "a+");
		passwd_file != NULL ? flag = 1 : tries++;
	}
	if (!flag) {
		log_action("Could not open or create passwords file");
		error(1, errno, "Could not open or create passwords file");
	}
	else {
		log_action("Passwords file opened");
		flag = 0;
		tries = 0;
	}


	while((tries < MAX_TRIES) && !flag ) {		//create socket
		log_action("Trying to create socket");
		main_socket = socket(AF_INET, SOCK_STREAM|SOCK_NONBLOCK, 0);	//nonblcking socket because we don't want to wait checking for incoming connections
		main_socket != -1 ? flag = 1 : tries++;
	}
	if (!flag) {
		log_action("Can't create socket");
		error(1, errno, "Can't create socket");
	}
	else {
		log_action_num("Socket created, descriptor", main_socket);
		flag = 0;
		tries = 0;
	}


		//prepare to bind socket
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	serv_addr.sin_port = htons(PORT); 


	while((tries < MAX_TRIES) && !flag ) {		//bind socket
		log_action("Try to bind socket");
		status = bind(main_socket, (struct sockaddr*)&serv_addr, sizeof(serv_addr));
		status != -1 ? flag = 1 : tries++;
	}
	if (!flag) {
		log_action("Can't bind socket");
		error(1, errno, "Can't bind socket");
	}
	else {
		log_action("Socket binded");
		flag = 0;
		tries = 0;
	}


	while((tries < MAX_TRIES) && !flag ) {		//listen on the socket
		log_action("Trying to listen on the socket");
		status = listen(main_socket, MAX_THREADS);	//we won't accept more clients than we can handle
		status != -1 ? flag = 1 : tries++;
	}
	if (!flag) {
		log_action("Can't listen on the socket");
		error(1, errno, "Can't listen on the socket");
	}
	else {
		log_action("Started listening on socket");
		flag = 0;
		tries = 0;
	}

		//we are done here
	return tmp_chat_log;
}
