#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <unistd.h>


#define FTYPE float //change this to double for greater precesion
#define PRINT_VALUES() printf("%f\n%f\n%f\n%f\n%f\n%f\n%f\n%f\n", \
		gamma0,\
		rho0,\
		P0,\
		U0,\
		gamma3,\
		rho3,\
		P3,\
		U3\
	)


void usage();
FTYPE sq(FTYPE x);	//for convenience
FTYPE input_fract(char* var_name, int quiet);
FTYPE input_exp(char* var_name, int quiet);


int main(int argc, char* argv[]) {

	int i, opt, var, debug, quiet;
	int coefs_only, intervals_only;

	FTYPE gamma0, rho0, P0, U0;
	FTYPE gamma3, rho3, P3, U3;
	FTYPE a[7];	//coefficients

	var = 1;	//default variant
	debug = 0;	//debugging off by default
	quiet = 0;	//prompt user for input
	coefs_only = 0;	//don't stop on coefs
	intervals_only = 0;	//don't stop on intervals

	while((opt = getopt(argc, argv, "12vhqci")) != -1) {
		switch(opt) {
		case '1':
			var = 1;
			break;

		case '2':
			var = 2;
			break;

		case 'v':
			debug = 1;
			break;

		case 'h':
			usage();
			exit(0);
			break;

		case 'q':
			quiet = 1;
			break;

		case 'c':
			coefs_only = 1;
			break;

		case 'i':
			intervals_only = 1;
			break;

		default:
			usage();
			exit(1);
		}
	}


	//time to enter values
	gamma0 = input_fract("gamma0", quiet);
	rho0 = input_exp("rho0", quiet);
	U0 = input_exp("U0", quiet);
	P0 = input_exp("P0", quiet);

	gamma3 = input_fract("gamma3", quiet);
	rho3 = input_exp("rho3", quiet);
	U3 = input_exp("U3", quiet);
	P3 = input_exp("P3", quiet);

	if(debug) {
		printf("Variables:\n");
		PRINT_VALUES();
	}

	FTYPE X, alpha0, alpha3, e0, e3, C0, C3;	//intermediate vars

	C0 = sqrt(gamma0*P0/rho0);
	C3 = sqrt(gamma3*P3/rho3);

	X = P3/P0;
	alpha0 = (gamma0 + 1)/(gamma0 - 1);
	alpha3 = (gamma3 + 1)/(gamma3 - 1);

	e0 = 2*sq(C0)/(gamma0*(gamma0 - 1)*sq(U3 - U0));
	e3 = 2*sq(C3)/(gamma3*(gamma3 - 1)*sq(U3 - U0));


	//calculating coefs
	if(var == 1) {
		a[0] = sq(alpha0*e3 - alpha3*X*e0);

		a[1] = 2*((alpha0*e3 - alpha3*X*e0)*(e3*(1 - 2*alpha0*X) \
			- e0*X*(X - 2*alpha3)) - alpha0*alpha3*X*(alpha0*e3 \
			+ alpha3*X*e0));

		a[2] = sq(e3)*(6*sq(alpha0)*sq(X) - 8*alpha0*X + 1) \
			- 2*e0*e3*X*(alpha0*alpha3*(sq(X) + 4*X + 1) \
			- 2*(X + 1)*(alpha3 + alpha0*X) + X) \
			+ sq(e0)*sq(X)*(6*sq(alpha3) - 8*alpha3*X + sq(X)) \
			+ sq(alpha0)*sq(alpha3)*sq(X) \
			- 2*alpha0*X*e3*(alpha0*X - 2*alpha0*alpha3*X + 2*alpha3) \
			- 2*alpha3*sq(X)*e0*(alpha3 + 2*alpha0*X - 2*alpha0*alpha3);

		a[3] = -2*X*(2*sq(e3)*(sq(alpha0)*sq(X) - 3*alpha0*X + 1) \
			+ e0*e3*((alpha3 + alpha0*X)*(sq(X) + 4*X + 1) \
			- 2*alpha0*alpha3*X*(X + 1) - 2*X*(X + 1)) \
			+ 2*sq(e0)*X*(sq(X) - 3*alpha3*X + sq(alpha3)) \
			- alpha0*alpha3*X*(alpha0*X + alpha3) \
			+ e3*(sq(alpha0)*alpha3*sq(X) - 2*X*(2*alpha0*alpha3 \
			+ sq(alpha0)*X) + (2*alpha0*X + alpha3)) \
			+ e0*X*(alpha0*sq(alpha3) - 2*alpha3*(alpha3 + 2*alpha0*X)\
			+ 2*alpha3*X + alpha0*sq(X)));

		a[4] = sq(X)*(sq(e3)*(sq(alpha0)*sq(X) - 8*alpha0*X + 6) \
			- 2*e0*e3*(alpha0*alpha3*X - 2*(X + 1)*(alpha3 + alpha0*X) \
			+ sq(X) + 4*X + 1) + sq(e0)*(sq(alpha3) - 8*alpha3*X + 6*sq(X)) \
			+ (sq(alpha3) + 4*alpha0*alpha3*X + sq(alpha0)*sq(X)) \
			- 2*e3*((sq(alpha0)*X + 2*alpha0*alpha3)*X - 2*(2*alpha0*X \
			+ alpha3) + 1) - 2*e0*(alpha3*(2*alpha0*X + alpha3) \
			- 2*X*(2*alpha3 + alpha0*X) + sq(X)));

		a[5] = 2*pow(X, 3)*(sq(e3)*(alpha0*X - 2) - e0*e3*(alpha0*X - 2 \
			+ alpha3 - 2*X) + sq(e0)*(alpha3 - 2*X) + (alpha3 + alpha0*X) \
			- e3*(2*alpha0*X + alpha3 - 2) - e0*(2*alpha3 + alpha0*X - 2*X));

		a[6] = pow(X, 4)*(sq(e3 - e0) + 1 - 2*(e3 + e0));
	}
	else if(var == 2) {
		printf("Not implemented yet.\n");
		exit(1);
	}
	else {
		printf("Mysterious error occured\n");
		exit(1);
	}

	if(debug | coefs_only) {	//print coefs
		for(i = 0; i < 7; i++)
			printf("a[%i] = %f\n", i, a[i]);
	}
	if(coefs_only)
		return(0);

	return 0;
}

void usage() {
	printf(
		"Available options:\n"
		"-1, -2 - select variant (1 by default)\n"
		"-v - print debugging information\n"
		"-h - print this help and exit\n"
		"-q - don't promt for input\n\n"
		"Data format:\n"
		"a/b for gamma (for example, \"5/3\"),\n"
		"a.bEc for everything else (for example 1.2E3).\n"
	);
}

FTYPE sq(FTYPE x) {
	return pow(x, 2);
}

FTYPE input_fract(char* var_name, int quiet) {

	FTYPE num, denom;

	if(!quiet)
		printf("%s = ", var_name);
	scanf("%f/%f", &num, &denom);

	return num/denom;
}

FTYPE input_exp(char* var_name, int quiet) {

	FTYPE mant, exp;

	if(!quiet)
		printf("%s = ", var_name);
	scanf("%f", &mant);

	return mant;
}
