
extern pthread_mutex_t idle_threads_mutex;
extern int idle_threads;

void idle_threads_inc();
void idle_threads_dec();
void create_new_thread(pthread_t* id, void *(*funct) (void *));
