#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <unistd.h>


#define FTYPE float //change this to double for greater precesion


void usage();
FTYPE sq(FTYPE x);	//for convenience
FTYPE input_fract(char* var_name, int quiet);
FTYPE input_exp(char* var_name, int quiet);


int main(int argc, char* argv[]) {

	int i, opt, var, debug, quiet;
	int coefs_only, intervals_only;

	FTYPE gamma0, rho0, P0, U0;
	FTYPE gamma3, rho3, P3, U3;
	FTYPE C3;
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
	if(var == 1) {
		gamma0 = input_fract("gamma0", quiet);
		rho0 = input_exp("rho0", quiet);
		U0 = input_exp("U0", quiet);
		P0 = input_exp("P0", quiet);

		gamma3 = input_fract("gamma3", quiet);
		rho3 = input_exp("rho3", quiet);
		U3 = input_exp("U3", quiet);
		P3 = input_exp("P3", quiet);
	}
	else if(var == 2) {
		gamma0 = input_fract("gamma0", quiet);
		rho0 = input_exp("rho0", quiet);
		P0 = input_exp("P0", quiet);
		U0 = input_exp("U0", quiet);

		gamma3 = input_fract("gamma3", quiet);
		C3 = input_exp("C3", quiet);
		P3 = input_exp("P3", quiet);
		U3 = input_exp("U3", quiet);
	}
	else {
		printf("Mysterious error occured\n");
		exit(1);
	}


	FTYPE X, alpha0, alpha3, e0, e3, C0;	//intermediate vars
	FTYPE n, mu, nu, Z, Y;

	//calculating intermediate vars
	if(var == 1) {
		C0 = sqrt(gamma0*P0/rho0);
		C3 = sqrt(gamma3*P3/rho3);

		X = P3/P0;
		alpha0 = (gamma0 + 1)/(gamma0 - 1);
		alpha3 = (gamma3 + 1)/(gamma3 - 1);

		e0 = 2*sq(C0)/(gamma0*(gamma0 - 1)*sq(U3 - U0));
		e3 = 2*sq(C3)/(gamma3*(gamma3 - 1)*sq(U3 - U0));
	}
	else if(var == 2) {
		rho3 = gamma3*P3/sq(C3);
		alpha0 = (gamma0 + 1)/(gamma0 - 1);
		n = 2*gamma3/(gamma3 - 1);
		mu = (U3 - U0)*sqrt((gamma0 - 1)*rho0/(2*P0));
		nu = 2/(gamma3 - 1)*sqrt(gamma3*(gamma0 - 1)/2 * P3/P0 * rho0/rho3);
		X = P3/P0;
	}
	else {
		printf("Mysterious error occured\n");
		exit(1);
	}


	//calculating coefs
	if(var == 1) {
		a[0] = sq(alpha0*e3 - alpha3*X*e0);	//Y^6

		a[1] = 2*((alpha0*e3 - alpha3*X*e0)*(e3*(1 - 2*alpha0*X) \
			- e0*X*(X - 2*alpha3)) - alpha0*alpha3*X*(alpha0*e3 \
			+ alpha3*X*e0));	//Y^5

		a[2] = sq(e3)*(6*sq(alpha0)*sq(X) - 8*alpha0*X + 1) \
			- 2*e0*e3*X*(alpha0*alpha3*(sq(X) + 4*X + 1) \
			- 2*(X + 1)*(alpha3 + alpha0*X) + X) \
			+ sq(e0)*sq(X)*(6*sq(alpha3) - 8*alpha3*X + sq(X)) \
			+ sq(alpha0)*sq(alpha3)*sq(X) \
			- 2*alpha0*X*e3*(alpha0*X - 2*alpha0*alpha3*X + 2*alpha3) \
			- 2*alpha3*sq(X)*e0*(alpha3 + 2*alpha0*X - 2*alpha0*alpha3);	//Y^4

		a[3] = -2*X*(2*sq(e3)*(sq(alpha0)*sq(X) - 3*alpha0*X + 1) \
			+ e0*e3*((alpha3 + alpha0*X)*(sq(X) + 4*X + 1) \
			- 2*alpha0*alpha3*X*(X + 1) - 2*X*(X + 1)) \
			+ 2*sq(e0)*X*(sq(X) - 3*alpha3*X + sq(alpha3)) \
			- alpha0*alpha3*X*(alpha0*X + alpha3) \
			+ e3*(sq(alpha0)*alpha3*sq(X) - 2*X*(2*alpha0*alpha3 \
			+ sq(alpha0)*X) + (2*alpha0*X + alpha3)) \
			+ e0*X*(alpha0*sq(alpha3) - 2*alpha3*(alpha3 + 2*alpha0*X)\
			+ 2*alpha3*X + alpha0*sq(X)));	//Y^3

		a[4] = sq(X)*(sq(e3)*(sq(alpha0)*sq(X) - 8*alpha0*X + 6) \
			- 2*e0*e3*(alpha0*alpha3*X - 2*(X + 1)*(alpha3 + alpha0*X) \
			+ sq(X) + 4*X + 1) + sq(e0)*(sq(alpha3) - 8*alpha3*X + 6*sq(X)) \
			+ (sq(alpha3) + 4*alpha0*alpha3*X + sq(alpha0)*sq(X)) \
			- 2*e3*((sq(alpha0)*X + 2*alpha0*alpha3)*X - 2*(2*alpha0*X \
			+ alpha3) + 1) - 2*e0*(alpha3*(2*alpha0*X + alpha3) \
			- 2*X*(2*alpha3 + alpha0*X) + sq(X)));	//Y^2

		a[5] = 2*pow(X, 3)*(sq(e3)*(alpha0*X - 2) - e0*e3*(alpha0*X - 2 \
			+ alpha3 - 2*X) + sq(e0)*(alpha3 - 2*X) + (alpha3 + alpha0*X) \
			- e3*(2*alpha0*X + alpha3 - 2) - e0*(2*alpha3 + alpha0*X - 2*X));	//Y

		a[6] = pow(X, 4)*(sq(e3 - e0) + 1 - 2*(e3 + e0));	//1
	}
	else if(var == 2) {
		a[0] = sq(X);	//Z^2n

		a[1] = -alpha0*sq(nu)*X;	//Z^(n+2)

		a[2] = 2*alpha0*nu*(mu + nu)*X;	//Z^(n + 1)

		a[3] = -(2 + sq(nu + mu)*alpha0)*X;	//Z^n

		a[4] = -sq(nu);	//Z^2

		a[5] = 2*nu*(mu + nu);	//Z

		a[6] = -sq(mu + nu) + 1;	//1
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
		"-q - don't promt for input\n"
		"-c - only calculate an print coefs.\n"
		"-i - only calculate and print intervals\n"
		"\n"
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
