#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>

struct borders_s {
	unsigned long int left;
	unsigned long int right;
	unsigned long int *sum;
};
typedef struct borders_s borders;


void* thread_function(void* limits) {
	int i;

	for(i = ((borders *)limits)->left; i <= ((borders *)limits)->right; i++) {
		*(((borders *)limits)->sum) += i;
	}

	printf("Left: %lu; right: %lu; sum: %lu\n", ((borders *)limits)->left, ((borders *)limits)->right, *(((borders *)limits)->sum) );

	return (void *)NULL;
}


int main(int argc, char *argv[]) {

	long unsigned int result = 0, left, right, *vars, len;
	int i, n;

	if(argc != 4) {
		printf("Usage: %s left right thread_amount\n", argv[0]);
		return 0;
	}

	sscanf(argv[1], "%lu", &left);
	sscanf(argv[2], "%lu", &right);
	sscanf(argv[3], "%i", &n);

	borders* thread_args = (borders *)malloc(n * sizeof(borders));
	pthread_t* threads = (pthread_t *)malloc(n * sizeof(pthread_t));
	vars = (long unsigned int *)malloc(n * sizeof(long unsigned int));

	len = right - left + 1;

	for(i = 0; i < n; i++) {
		thread_args[i].left = left;
		if(i < len%n) {
			thread_args[i].right = left + len/n + 1;
			left += len/n + 1;
		}
		else {
			thread_args[i].right = left + len/n;
			left += len/n;
		}

		thread_args[i].sum = &( vars[i] );
	}


	for(i = 0; i < n; i++)
		pthread_create(
			&(threads[i]),
			(pthread_attr_t *)NULL,
			thread_function,
			(void *)&(thread_args[i])
		);


	for(i = 0; i < n; i++)
		pthread_join( threads[i], (void **)NULL );

	for(i = 0; i < n; i++)
		result += vars[i];

	printf("Sum is: %lu\n", result);

	free(thread_args);
	free(threads);
	free(vars);
	return 0;
}
