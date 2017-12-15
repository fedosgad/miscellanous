


struct message_s {
	time_t timestamp;
	char* msg_text;
	int type;			//0 - text; 1 - command
	char* client_name;
	int msg_id;
};

typedef struct message_s msg_t;



int idle_threads;
msg_t* msg_list;
