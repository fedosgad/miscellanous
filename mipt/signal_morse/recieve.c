#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>

int k;

void handler_0() {
	k = 1;
	return;
}

void handler_1() {
	k = 2;
	return;
}

int main() {
	int tr_pid, flag = 0;
	int out = 0;
	k = 0;
	
	signal(SIGUSR1, handler_0);
	signal(SIGUSR2, handler_1);

	printf("Reciever PID is: %i\n", getpid());
	scanf("%i", &tr_pid);

	while(1) {
		while(1) {
			switch(k) {
				case 0:
					break;
				case 1:
					out >>= 1;
					flag++;
					k = 0;		//all manipulations with k MUST be finished before kill()
//					printf("0");
					kill(tr_pid, SIGUSR1);
					break;
				case 2:
					out >>= 1;
					out |= (1 << 7);
					flag++;
					k = 0;		//so there MUST NOT be any modifications of k before next switch(k)
//					printf("1");
					kill(tr_pid, SIGUSR1);
					break;
			}
			if(flag == 8) break;
		}
//	printf("\n");

	printf("%c", (char)out);			//if there's no cast (and out is char), result is always backwards, idk why

	out = 0;
	flag = 0;
	}

	return 0;
}
