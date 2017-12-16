#include <stdio.h>
#include <time.h>
#include <error.h>
#include <errno.h>
#include <unistd.h>	//close()
#include <sys/types.h>
#include <sys/socket.h>

#include "client_interaction.h"
#include "debug.h"


void* handle_connection(void* arg) {

	int client_socket;
	int flag = 0, status, buf = 0;
	msg_t* last_msg_local;


	while(!flag) {		//try to accept connection
		client_socket = accept(main_socket, NULL, NULL);
		if( client_socket != -1 )
			flag = 1;
		else 
			if( !((errno == EAGAIN)||(errno == EWOULDBLOCK)) )		//if there are connections and error occures
				log_action("Thread failed to accept connection");
	}
	flag = 0;
	log_action_num("Accepted connection on socket, descriptor", client_socket);

	while(!flag){		//receive number
		status = recv(client_socket, (void*)&buf, (size_t)sizeof(int), 0);
		if (status == -1) {
			if( !((errno == EAGAIN)||(errno == EWOULDBLOCK)) )
				log_action("Thread failed to receive");
		}
		else
			flag = 1;
	}
	flag = 0;
	log_action_num("Thread received number", buf);


	while(!flag) {		//close client_socket
		status = close(client_socket);
		if(status == -1) {
			error(0, errno, "Thread failed to close socket");
			log_action_num("Thread failed to close socket", client_socket);
		}
		else
			flag = 1;
	}
	flag = 0;
	log_action_num("Thread closed socket", client_socket);

	return NULL;

}
