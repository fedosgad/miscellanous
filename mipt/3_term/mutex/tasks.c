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


	while( !flag ) {

		flag = 1;

		for(i = 0; i < TASKS; i++) {
			pthread_mutex_lock( &lock );
			if(tasks[i].status == NEW) {

				printf("Thread ID %u, %i tasks already done, taking task ID %i\n", (unsigned int)pthread_self(), *((int *)inp), tasks[i].id);

				tasks[i].status = PROCESSING;
				tasks[i].worker = pthread_self();
				pthread_mutex_unlock( &lock );

				usleep( tasks[i].time );

				tasks[i].status = DONE;
				(*((int *)inp))++;
				flag = 0;
				break;
			}
			else pthread_mutex_unlock( &lock );
		}
	}

	return (void*)NULL;
}


int main(int argc, char* argv[]) {
	int i, total_tasks_done = 0, *thread_ret;
	pthread_t workers_list[THREADS];
	pthread_mutex_init( &lock, NULL );

	srand( (int)time( NULL ) );

	thread_ret = (int *)malloc(THREADS*sizeof(int));

	for(i = 0; i < THREADS; i++)
		thread_ret[i] = 0;


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
				(void *)&(thread_ret[i])
		);
	}

	for(i = 0; i < THREADS; i++ )
		pthread_join( workers_list[i], (void **)NULL );

	for(i = 0; i < THREADS; i++ )
		total_tasks_done += thread_ret[i];

	printf("Total tasks done: %i\n", total_tasks_done);

	pthread_mutex_destroy( &lock );
	free(thread_ret);

	return 0;
}
