#define PORT 12345
#define MAX_THREADS 100
#define MAX_TRIES 10

#define PASSWD_FILE "passwd.db"
#define CHAT_LOG_FILE "chat_log"
#define SERVER_LOG_FILE "server_log"
#define INITIAL_QUEUE_KEY 125


void* handle_connection(void* arg);		//thread body
FILE* init();				//returns tmp chat log descriptor
pthread_t* shrink_threads_array(void* array, int length, pthread_t id);
void deinit(pthread_t* thread_id, FILE* tmp_chat_log);
void sigint_handler(int signum);

static int main_stop;

pthread_mutex_t idle_threads_mutex;
pthread_mutex_t server_log_file_mutex;
pthread_mutex_t passwd_file_mutex;
pthread_mutex_t messages_mutex;


int main_socket;
int join_request_qd;
FILE* server_log_file;
FILE* passwd_file;
