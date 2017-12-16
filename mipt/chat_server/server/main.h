#define PORT 12345
#define MAX_THREADS 100
#define MAX_TRIES 10

#define PASSWD_FILE "passwd.db"
#define CHAT_LOG_FILE "chat_log"
#define SERVER_LOG_FILE "server_log"


void* handle_connection(void* arg);		//thread body
FILE* init();				//returns tmp chat log descriptor
//~ void deinit();


int main_socket;
FILE* server_log_file;
FILE* passwd_file;
