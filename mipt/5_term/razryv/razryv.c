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

	int i, opt, var, verbose, quiet, method;
	int coefs_only, intervals_only, roots_only, answers_only;

	int segments;


	interval tmp_int;

	var = 1;	//set default variant

	verbose = 0;	//verbose off by default
	quiet = 0;	//prompt user for input
	coefs_only = 0;	//don't stop on coefs
	intervals_only = 0;	//don't stop on intervals
	roots_only = 0;	//don't stop on roots

	segments = 10000;
	FTYPE epsilon = 1e-6;

	while((opt = getopt(argc, argv, "12vhqcirs:e:")) != -1) {
		switch(opt) {
		case '1':
			var = 1;
			break;

		case '2':
			var = 2;
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

		case 'e':
			epsilon = atof(optarg);
			break;

		case 'r':
			roots_only = 1;
			break;

		default:
			usage();
			exit(1);
		}
	}

	set_var(var);

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
			printf("a[%i] = %Le\n", i, get_a(i));
	}
	if(coefs_only)
		return(0);

	if(verbose && (var == 2)) {
		newline();
		printf("n = %Lf\n", get_n());
	}

	estimate_amount();
	initial_guess();

	if(verbose) {
		newline();
		printf("A = %Le\n", get_A());
		printf("B = %Le\n", get_B());
	}
	if(verbose) {
		tmp_int = get_roots_int(1);
		newline();
		printf("Initial positive interval: ");
		print_int(tmp_int);
		newline();
		printf("Values:");
		if(var == 1) {
			printf("[ %Le; %Le ]\n", fval_1(tmp_int.l), fval_1(tmp_int.r));
		}
		else {
			printf("[ %Le; %Le ]\n", fval_2(tmp_int.l), fval_2(tmp_int.r));
		}
		printf("No more than %i positive roots\n", get_roots_amount(1));

		tmp_int = get_roots_int(2);
		newline();
		printf("Initial negative interval: ");
		print_int(tmp_int);
		newline();
		printf("Values:");
		if(var == 1) {
			printf("[ %Le; %Le ]\n", fval_1(tmp_int.l), fval_1(tmp_int.r));
		}
		else {
			printf("[ %Le; %Le ]\n", fval_2(tmp_int.l), fval_2(tmp_int.r));
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

	find_roots(epsilon);

	if(verbose) {
		newline();
		printf("Roots:\n");
	}
	if(verbose | roots_only) {
		for(i = 0; i < get_roots_amount(0); i++)
			printf("%.10Le\n", roots[i]);
	}
	if(roots_only) {
		free(roots);
		return 0;
	}

	calc_answers();

	if(verbose) {
		newline();
		printf("Answers:\n");
	}
	if(var == 1) {
		for(i = 0; i < get_total_answers(); i++) {
			if(i%2 == 0)
				printf("D0 = %Le\n", answers[i]);
			else {
				printf("D3 = %Le\n", answers[i]);
				printf("Lambda_1 = %Le\n", lambda[i - 1]);
				printf("Lambda_2 = %Le\n", lambda[i]);
			}

			if(i%4 == 3)
				printf("a_cr^2 = %Le\n", a_cr[i/4]*a_cr[i/4]);

			if(i%4 == 3)
				newline();
		}
	}
	else {
		for(i = 0; i < get_total_answers(); i++) {
			printf("D0 = %Le\n", answers[i]);
			printf("Lambda_1 = %Le\n", lambda[2*i]);
			printf("Lambda_2 = %Le\n", lambda[2*i + 1]);

			if(i%2 == 1)
				printf("a_cr^2 = %Le\n", a_cr[i/2]*a_cr[i/2]);

			if(i%2 == 1)
				newline();
		}
	}

	free(roots);
	free(root_int);
	free(answers);
	free(a_cr);
	free(lambda);

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
		"-r - only calculate and print roots\n"
		"-s [int] - divide initial guess into [int] segments (10000 by default)\n"
		"-e [float] - find roots with [float] precesion (1r-6 by default)\n"
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
	scanf("%Lf/%Lf", &num, &denom);

	return num/denom;
}

FTYPE input_exp(char* var_name, int quiet) {

	FTYPE mant, exp;

	if(!quiet)
		printf("%s = ", var_name);
	scanf("%Lf", &mant);

	return mant;
}

void print_int(interval inp) {
	printf("[ %Le; %Le ]", inp.l, inp.r);
	return;
}

void newline() {
	printf("\n");
}

