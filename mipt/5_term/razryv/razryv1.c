#include <stdio.h>
#include <math.h>

#define FTYPE float //change this to double for greater precesion


void usage();


int main(int argc, char* argv[]) {

	FTYPE gamma0, rho0, P0, U0;
	FTYPE gamma3, rho3, P3, U3;

	FTYPE tmp, tmp2;

	usage();

	printf("gamma0 = ");
	scanf("%f/%f", &tmp, &tmp2);
	gamma0 = tmp/tmp2;

	printf("%f\n", gamma0);

	return 0;
}


void usage() {
	printf(
		"Enter data. Format:\n"
		"a/b for gamma (for example, \"5/3\"),\n"
		"a.bEc for everything else (for example 1.2E3).\n"
	);
}
