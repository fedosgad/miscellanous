#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <error.h>
#include <errno.h>
#include <pthread.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <signal.h>

#include "main.h"
#include "tui.h"
#include "debug.h"


int main(int argc, char* argv[]) {

	pthread_t* tui_thread;
	int number, flag = 0, status;
	main_stop = 0;

	if(argc != 3)
		error(1, 0, "Usage: %s <IP> <Port>", argv[0]);

	init(argv);

	while(main_stop != 1) {
		scanf("%i", &number);
		if(number == 0) {
			main_stop = 1;
			flag = 1;
		}
		while(!flag) {
			status = send(main_socket, &number, sizeof(int), MSG_CONFIRM);
			status != -1 ? flag = 1 : log_action("Can't send number");
		}
		flag = 0;
	}

	number = 0;
	while(!flag) {
		status = send(main_socket, &number, sizeof(int), MSG_CONFIRM);
		status != -1 ? flag = 1 : log_action("Can't send number");
	}
	flag = 0;

	deinit();

	return 0;
}


void init(char* argv[]) {

	int tries = 0, flag = 0, status;
	struct sockaddr_in serv_addr;

	signal(SIGINT, sigint_handler);
/* Whatever we try to do here, we try to do it MAX_TRIES times 
 * We log every step if we can (if we have log file)
 */

	while((tries < MAX_TRIES) && !flag ) {		//log file itself
		printf("Trying to open log file\n");
		client_log_file = fopen(CLIENT_LOG_FILE, "a");
		client_log_file != NULL ? flag = 1 : tries++;
	}
	if (!flag) 
		error(1, errno, "Can't open or create log file");
	flag = 0;
	tries = 0;
	setlinebuf(client_log_file);		//if we don't, we'll never see our logs
	log_action("Client started");


	while((tries < MAX_TRIES) && !flag ) {		//create socket
		log_action("Trying to create socket");
		main_socket = socket(AF_INET, SOCK_STREAM, 0);
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


		//prepare socket for connect()
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_port = htons(atoi(argv[2]));

	if( inet_pton(AF_INET, argv[1], &(serv_addr.sin_addr)) != 1)
		error(1, errno, "Can't convert IP address");	//and we die here


	while((tries < MAX_TRIES) && !flag ) {		//connect to server
		log_action("Trying to connect");
		status = connect(main_socket, (struct sockaddr *)&serv_addr, sizeof(serv_addr));
		status != -1 ? flag = 1 : tries++;
	}
	if (!flag) {
		log_action("Can't connect");
		error(1, errno, "Can't connect");
	}
	else {
		log_action("Socket connected");
		flag = 0;
		tries = 0;
	}

		//we are done here
	return;
}

void server_interaction() {

	

	return;
}

void deinit() {

	int flag = 0, status;

	while(!flag) {		//close client_socket
		status = close(main_socket);
		if(status == -1) {
			error(0, errno, "Failed to close socket");
			log_action_num("Failed to close socket", main_socket);
		}
		else
			flag = 1;
	}
	flag = 0;
	log_action_num("Closed main socket", main_socket);

	fclose(client_log_file);
	printf("Client_log_closed\n");

	return;
}

void sigint_handler(int signum) {
	main_stop = 1;
	return;
}
