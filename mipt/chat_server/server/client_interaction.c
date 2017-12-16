#include <stdio.h>
#include <time.h>
#include <error.h>
#include <errno.h>
#include <unistd.h>	//close()
#include <sys/types.h>
#include <sys/socket.h>
#include <pthread.h>
#include <sys/ipc.h>
#include <sys/msg.h>

#include "client_interaction.h"
#include "debug.h"
#include "misc.h"


void* handle_connection(void* arg) {

	int client_socket;
	int flag = 0, status, buf = 0;
	msg_t* last_msg_local;
	join_request join_buf;


	join_buf.type = 1;
	join_buf.id = pthread_self();


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

	pthread_mutex_lock(&idle_threads_mutex);
	idle_threads -= 1;
	pthread_mutex_unlock(&idle_threads_mutex);

	while(!flag){		//receive number (payload)
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

	while(!flag) {		//send join request
		status = msgsnd(join_request_qd, (void*)&join_buf, sizeof(pthread_t), 0);
		status == 0 ? flag = 1 : log_action_num("Can't send join message, thread id", pthread_self());
	}


	pthread_mutex_lock(&idle_threads_mutex);
	idle_threads += 1;
	pthread_mutex_unlock(&idle_threads_mutex);

	log_action_num("Thread joins, id", (ulong)pthread_self());
	return NULL;

}
