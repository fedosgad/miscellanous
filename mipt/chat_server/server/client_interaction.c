#include <stdio.h>
#include <time.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/socket.h>

#include "client_interaction.h"
#include "debug.h"


void* handle_connection(void* arg) {

	int client_socket;
	int flag = 0;
	msg_t* last_msg_local;


	while(!flag) {
		client_socket = accept(main_socket, NULL, NULL);
		if( client_socket != -1 )
			flag = 1;
		else 
			if( !((errno == EAGAIN)||(errno == EWOULDBLOCK)) )
				log_action("Thread failed to accept connection");
	}
	flag = 0;
	log_action_num("Accepted connection on socket, descriptor", client_socket);

	


	

	return NULL;

}
