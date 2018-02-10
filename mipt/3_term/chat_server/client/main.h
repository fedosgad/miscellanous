
#define CLIENT_LOG_FILE "client_log"

#define MAX_TRIES 10


FILE* client_log_file;
int main_socket;
int main_stop;


void init(char* argv[]);
void server_interaction();
void deinit();
void sigint_handler(int signum);
