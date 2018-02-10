#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>

int k;

void handler() {
	k = 1;
	return;
}

int main() {
	int rec_pid, i;
	char inp;

	k = 0;

	signal(SIGUSR1, handler);

	printf("Transmitter PID is: %i\n", getpid());
	scanf("%i\n", &rec_pid);

	while(1) {
		scanf("%c", &inp);

		for(i = 0; i < 8; i++) {
			if((inp % 2) == 0) kill(rec_pid, SIGUSR1);
			else kill(rec_pid, SIGUSR2);
			inp >>= 1;
			while(1) if(k) break;
			k = 0;
		}
	}

	return 0;
}
