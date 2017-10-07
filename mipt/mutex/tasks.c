#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <stdlib.h>
#include <time.h>


#define TASKS 100
#define THREADS 10



struct task_s {
	int time;
	int id;
	pthread_t worker;
	enum {
		NEW,
		PROCESSING,
		DONE
	} status;

};
typedef struct task_s task;


task tasks[TASKS];

pthread_mutex_t lock;


void* thread_body( void* inp ) {

	int i, flag = 0;
	static int tasks_done = 0;


	while( !flag ) {

		flag = 1;

		for(i = 0; i < TASKS; i++) {
			if(tasks[i].status == NEW) {
				if(pthread_mutex_trylock( &lock ) == 0) {

					printf("Thread ID %u, %i tasks already done, taking task ID %i\n", (unsigned int)pthread_self(), tasks_done, tasks[i].id);

					tasks[i].status = PROCESSING;
					tasks[i].worker = pthread_self();
					pthread_mutex_unlock( &lock );

					usleep( tasks[i].time );

					tasks[i].status = DONE;
					tasks_done++;
					flag = 0;
					break;
				}
				else ;
			}
		}
	}

	return (void*)&tasks_done;
}


int main(int argc, char* argv[]) {
	int i, total_tasks_done = 0, *thread_ret;
	pthread_t workers_list[THREADS];
	pthread_mutex_init( &lock, NULL );

	srand( (int)time( NULL ) );

	for(i = 0; i < TASKS; i++) {
		tasks[i].time = rand()%1000;
		tasks[i].id = i;
		tasks[i].status = NEW;
	}


	for(i = 0; i < THREADS; i++ ) {
		pthread_create(
				&(workers_list[i]),
				(pthread_attr_t *)NULL,
				thread_body,
				(void *)NULL
		);
	}

	for(i = 0; i < THREADS; i++ ) {
		pthread_join( workers_list[i], (void **)&thread_ret );
		total_tasks_done += *thread_ret;
	}

	printf("Total tasks done: %i\n", total_tasks_done);

	pthread_mutex_destroy( &lock );

	return 0;
}
