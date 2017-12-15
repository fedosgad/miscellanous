#include <stdio.h>	//general IO
#include <stdlib.h>	//malloc()
#include <unistd.h>
#include <pthread.h>	//threads
#include <error.h>	//error()
#include <errno.h>	//error codes
#include <time.h>	//timestamps
#include <sys/types.h>	//because required
#include <sys/socket.h>	//socket functions
#include <arpa/inet.h>	//htonl() and struct sockaddr_in


#include "client_interaction.h"


#define PORT 12345
#define MAX_THREADS 100
#define MAX_TRIES 10

#define PASSWD_FILE "passwd.db"
#define CHAT_LOG_FILE "chat_log"
#define SERVER_LOG_FILE "server_log"



//void* handle_connection(void* );
FILE* init();				//returns tmp chat log descriptor
void log_action(const char* text);
//~ void deinit();


extern int idle_threads;
extern msg_t* msg_list;		//connected list of messages

int socketfd;
FILE* server_log_file;


int main(int argc, char* argv[]) {

	struct sockaddr_in serv_addr;
	FILE* tmp_chat_log;
	pthread_t *thread_id;

	int total_threads = 0;

	tmp_chat_log = init();


	//~ deinit();

	return 0;
}


void log_action(const char* text) {

	printf("%s\n", text);
	fprintf(server_log_file, "%ld ", (long int)time(NULL));		//timestamp
	fprintf(server_log_file, "%s\n", text);

	return;

}


FILE* init() {

	int tries = 0, flag = 0, status;
	struct sockaddr_in serv_addr;
	FILE* tmp_chat_log;

/* Whatever we try to do here, we try to do it MAX_TRIES times 
 * We log every step if we can (if we have log file)
 */

	while((tries < MAX_TRIES) && !flag ) {		//log file itself
		printf("Trying ot open log file\n");
		server_log_file = fopen(SERVER_LOG_FILE, "a");
		server_log_file != NULL ? flag = 1 : tries++;
	}
	if (!flag) {
		error(1, errno, "Can't open or create log file");
	}
	else {
		log_action("Server started");
		flag = 0;
		tries = 0;
	}

	while((tries < MAX_TRIES) && !flag ) {		//tmp log file for chat messages
		tmp_chat_log = tmpfile();
		tmp_chat_log != NULL ? flag = 1 : tries++;
		log_action("Trying to open temporary chat log");
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


/* TODO: password file
	while((tries < MAX_TRIES) && !flag ) {
		
		 ? flag = 1 : tries++;
		log_action("");
	}
	if (!flag) {
		log_action("")
		error(1, errno, "");
	}
	else {
		log_action("");
		flag = 0;
		tries = 0;
	}
*/


	while((tries < MAX_TRIES) && !flag ) {		//create socket
		socketfd = socket(AF_INET, SOCK_STREAM, 0);
		socketfd != -1 ? flag = 1 : tries++;
		log_action("Trying to create socket");
	}
	if (!flag) {
		log_action("Can't create socket");
		error(1, errno, "Can't create socket");
	}
	else {
		log_action("Socket created");
		flag = 0;
		tries = 0;
	}


		//prepare to bind socket
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	serv_addr.sin_port = htons(PORT); 


	while((tries < MAX_TRIES) && !flag ) {		//bind socket
		status = bind(socketfd, (struct sockaddr*)&serv_addr, sizeof(serv_addr));
		status != -1 ? flag = 1 : tries++;
		log_action("Try to bind socket");
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
		status = listen(socketfd, MAX_THREADS);	//we won't accept more clients than we can handle
		status != -1 ? flag = 1 : tries++;
		log_action("Trying to listen on the socket");
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
