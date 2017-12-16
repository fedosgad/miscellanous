extern FILE* server_log_file;

extern pthread_mutex_t idle_threads_mutex;
extern pthread_mutex_t server_log_file_mutex;
extern pthread_mutex_t passwd_file_mutex;
extern pthread_mutex_t messages_mutex;


void log_action(const char* text);
void log_action_num(const char* text, long int num);
