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
#include <sys/ipc.h>
#include <sys/msg.h>//message queues

#include "main.h"
#include "client_interaction.h"
#include "debug.h"
#include "misc.h"

int main(int argc, char* argv[]) {

	struct sockaddr_in serv_addr;
	FILE* tmp_chat_log;
	pthread_t *thread_id;
	join_request incoming_join_request;

	int total_threads = 0, status, flag = 0, too_much_threads = 0;

	idle_threads = 0;
	tmp_chat_log = init();

	pthread_mutex_init(&idle_threads_mutex, NULL);
	pthread_mutex_init(&server_log_file_mutex, NULL);
	pthread_mutex_init(&passwd_file_mutex, NULL);
	pthread_mutex_init(&messages_mutex, NULL);

	thread_id = (pthread_t*)malloc(sizeof(pthread_t));	//create 1 thread
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
	log_action_num("Created first thread, id", (ulong)thread_id[0]);
	total_threads = 1;

	pthread_mutex_lock(&idle_threads_mutex);
	idle_threads += 1;
	pthread_mutex_unlock(&idle_threads_mutex);

	log_action("Enter main loop");
	while(!flag) {
		if(idle_threads != 1) {		//thread management
			if( (idle_threads < 1)&&(total_threads <= MAX_THREADS) ) {	//not enogh threads - WE MUST CONSTRUCT ADDITIONAL THREADS (if we can)
				thread_id = (pthread_t*)realloc((void*)thread_id, (total_threads + 1)*sizeof(pthread_t));
				total_threads++;	//better check that everything is OK
				idle_threads_inc();
			}
			else if( (idle_threads < 1) && 	//too much threads
					(total_threads >= MAX_THREADS) &&
					(too_much_threads == 0)) {
				log_action("Can't create more threads - limit reached");//"not enough threads" case done
				too_much_threads = 1;
			}
			else if(idle_threads > 1) {
				while(!flag) {		//get join request
					status = msgrcv(join_request_qd,
									(void *)&incoming_join_request,
									sizeof(pthread_t),
									1,
									0);
					status != -1 ? flag = 1 : log_action("Waiting for join message");
				}
				flag = 0;
				//now join and realloc
				thread_id = shrink_threads_array((void *)thread_id, total_threads, incoming_join_request.id);
				total_threads -= 1;
				idle_threads_dec();
				log_action_num("Total threads now", total_threads);
				log_action_num("Idle threads now", idle_threads);
			}
		//thread management done
		}

		
	}


	pthread_join(thread_id[0], (void**)NULL);
	free(thread_id);

	pthread_mutex_destroy(&idle_threads_mutex);
	pthread_mutex_destroy(&server_log_file_mutex);
	pthread_mutex_destroy(&passwd_file_mutex);
	pthread_mutex_destroy(&messages_mutex);

	//~ deinit();


	return 0;
}


FILE* init() {

	int tries = 0, flag = 0, status;
	struct sockaddr_in serv_addr;
	FILE* tmp_chat_log;
	key_t join_request_queue;

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


	join_request_queue = INITIAL_QUEUE_KEY;
	while((tries < MAX_TRIES) && !flag ) {		//message queue for join requests
		log_action("Try to open or create message queue");
		join_request_qd = msgget(join_request_queue + tries, 0660|IPC_CREAT);
		join_request_qd != -1 ? flag = 1 : tries++;
	}
	if (!flag) {
		log_action("Could not open or create message queue");
		error(1, errno, "Could not open or create message queue");
	}
	else {
		log_action_num("Message queue opened, descriptor", join_request_qd);
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


pthread_t* shrink_threads_array(void* array, int length, pthread_t id) {

	pthread_t* new_array;
	int flag = 0, shift_from = 0, i;

	log_action_num("Called shrink threads for id", (ulong)id);

	while(shift_from < length) {		//find thread to join
		if(((pthread_t*)array)[shift_from] == id)
			break;
		shift_from++;
	}
	if(shift_from >= length)
		error(1, 0, "Can't join thread %lu - id not found", (ulong)id);

	while(!flag) {		//join found thread
		if(pthread_join(id, (void **)NULL) != 0) {
			error(0, errno, "Can't join thread %lu", (ulong)id);
			log_action_num("Can't join thread", (ulong)id);
		}
		else
			flag = 1;
	}
	flag = 0;
	
	log_action_num("Thread joined, id", (ulong)id);

	for(i = shift_from; i < length - 1; i++)	//shift remaining threads to the beginning
		((pthread_t*)array)[i] = ((pthread_t*)array)[i + 1];

	while(!flag) {		//realloc
		new_array = (pthread_t*)realloc(array, (length - 1)*sizeof(pthread_t));
		if(new_array != NULL)
			flag = 1;
	}
	flag = 0;

	return new_array;

}

