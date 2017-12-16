struct message_s {
	time_t timestamp;
	char* msg_text;
	int type;			//0 - text; 1 - command
	char* client_name;
	int msg_id;
};
typedef struct message_s msg_t;

struct join_request_s {
	long int type;	//always 1
	pthread_t id;
};
typedef struct join_request_s join_request;

//~ struct thread_arg_s {};
//~ typedef struct thread_arg_s thread_arg;

//~ struct thread_ret_s {};
//~ typedef struct thread_ret_s thread_ret;

int idle_threads;
msg_t* msg_list;		//list (array) of messages

extern int main_socket;
extern FILE* server_log_file;
extern int join_request_qd;


void* handle_connection(void* arg);
