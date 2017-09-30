#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>

struct borders_s {
	unsigned long int left;
	unsigned long int right;
};
typedef struct borders_s borders;


void* thread_function(void* limits) {
	int i;

	for(i = ((borders *)limits)->left; i < ((borders *)limits)->right; i++) {
		sum += i;
		printf("In thread sum = %lu\n", sum);
	}

	return (void *)&sum;
}


int main(int argc, char *argv[]) {

	long unsigned int result = 0, left, right, *ret_ptr, *;
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
	

	for(i = 0; i < n; i++) {
		if(i <= (right - left)%n) {
			thread_args[i].left = left + i*( (right - left)/n + 1 );
			thread_args[i].right = left + (i + 1)*( (right - left)/n + 1 );
		}
		else {
			thread_args[i].left = left + i*( (right - left)/n );
			thread_args[i].left = left + i*( (right - left)/n );
		}
	}

	for(i = 0; i < n; i++)
		pthread_create(
			&(threads[i]),
			(pthread_attr_t *)NULL,
			thread_function,
			(void *)&(thread_args[i])
		);


	for(i = 0; i < n; i++) {
		pthread_join( threads[i], (void **)&ret_ptr );
		result += *ret_ptr;
	}

	printf("Sum is: %lu\n", result);

	free(thread_args);
	free(threads);
	return 0;
}
