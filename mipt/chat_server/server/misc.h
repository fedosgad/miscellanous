
extern pthread_mutex_t idle_threads_mutex;
extern int idle_threads;

void idle_threads_inc();
void idle_threads_dec();
