#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "calculations.h"


void usage();
FTYPE input_fract(char* var_name, int quiet);	//for inputting fractions
FTYPE input_exp(char* var_name, int quiet);
void print_int(interval inp);
void newline();


int main(int argc, char* argv[]) {

	int i, opt, var, verbose, quiet;
	int coefs_only, intervals_only;

	int segments;

	interval tmp_int;

	set_var(1);	//set default variant

	verbose = 0;	//verbose off by default
	quiet = 0;	//prompt user for input
	coefs_only = 0;	//don't stop on coefs
	intervals_only = 0;	//don't stop on intervals

	segments = 10000;

	while((opt = getopt(argc, argv, "12vhqcis:")) != -1) {
		switch(opt) {
		case '1':
			set_var(1);
			break;

		case '2':
			set_var(2);
			break;

		case 'v':
			verbose = 1;
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

		case 's':
			segments = atoi(optarg);
			break;

		default:
			usage();
			exit(1);
		}
	}

	//enter values
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
	else {
		gamma0 = input_fract("gamma0", quiet);
		rho0 = input_exp("rho0", quiet);
		P0 = input_exp("P0", quiet);
		U0 = input_exp("U0", quiet);

		gamma3 = input_fract("gamma3", quiet);
		C3 = input_exp("C3", quiet);
		P3 = input_exp("P3", quiet);
		U3 = input_exp("U3", quiet);
	}

	//calculate coefficients
	calc_interm_vars();
	calc_coefs();

	if(verbose | coefs_only) {	//print coefs
		for(i = 0; i < 7; i++)
			printf("a[%i] = %e\n", i, a[i]);
	}
	if(coefs_only)
		return(0);

	if(verbose && (var == 2)) {
		newline();
		printf("n = %f\n", get_n());
	}

	estimate_amount();
	initial_guess();

	if(verbose) {
		newline();
		printf("A = %e\n", get_A());
		printf("B = %e\n", get_B());
	}
	if(verbose) {
		tmp_int = get_roots_int(1);
		newline();
		printf("Initial positive interval: ");
		print_int(tmp_int);
		newline();
		printf("Values:");
		if(var == 1) {
			printf("[ %e; %e ]\n", fval_1(tmp_int.l, a), fval_1(tmp_int.r, a));
		}
		else {
			printf("[ %e; %e ]\n", fval_2(tmp_int.l, a), fval_2(tmp_int.r, a));
		}
		printf("No more than %i positive roots\n", get_roots_amount(1));

		tmp_int = get_roots_int(2);
		newline();
		printf("Initial negative interval: ");
		print_int(tmp_int);
		newline();
		printf("Values:");
		if(var == 1) {
			printf("[ %e; %e ]\n", fval_1(tmp_int.l, a), fval_1(tmp_int.r, a));
		}
		else {
			printf("[ %e; %e ]\n", fval_2(tmp_int.l, a), fval_2(tmp_int.r, a));
		}
		printf("No more than %i negative roots\n", get_roots_amount(2));
	}

	find_root_ints(segments);

	if(verbose) {
		newline();
		printf("%i root intervals found:\n", get_roots_amount(0));
	}
	if(verbose | intervals_only) {
		for(i = 0; i < get_roots_amount(0); i++) {
			print_int(root_int[i]);
			newline();
		}
	}
	if(intervals_only) {
		free(root_int);
		return 0;
	}

	free(root_int);

	return 0;
}


void usage() {
	printf(
		"Available options:\n"
		"-1, -2 - select variant (1 by default)\n"
		"-v - print intermediate information\n"
		"-h - print this help and exit\n"
		"-q - don't promt for input\n"
		"-c - only calculate an print coefs.\n"
		"-i - only calculate and print intervals\n"
		"-s [n] - divide initial guess into n segments (10000 by default)\n"
		"\n"
		"Data format:\n"
		"a/b for gamma (for example, \"5/3\"),\n"
		"a.bEc for everything else (for example 1.2E3).\n"
	);
}

FTYPE input_fract(char* var_name, int quiet) {

	FTYPE num, denom;

	if(!quiet)
		printf("%s = ", var_name);
	scanf("%lf/%lf", &num, &denom);

	return num/denom;
}

FTYPE input_exp(char* var_name, int quiet) {

	FTYPE mant, exp;

	if(!quiet)
		printf("%s = ", var_name);
	scanf("%lf", &mant);

	return mant;
}

void print_int(interval inp) {
	printf("[ %e; %e ]", inp.l, inp.r);
	return;
}

void newline() {
	printf("\n");
}

