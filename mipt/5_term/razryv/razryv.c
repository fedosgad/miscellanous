#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <unistd.h>


#define FTYPE double //change this to double for greater precesion

struct interval {
	FTYPE l;
	FTYPE r;
};

typedef struct interval interval;

void usage();
FTYPE sq(FTYPE x);	//for convenience
FTYPE input_fract(char* var_name, int quiet);	//for inputting fractions
FTYPE input_exp(char* var_name, int quiet);
FTYPE max_abs(FTYPE* arr, int length);
FTYPE fval_1(FTYPE x, FTYPE* coef);	//calculate function value for var. 1
FTYPE fval_2(FTYPE x, FTYPE* coef, FTYPE n);	//same for var. 2
void print_int(interval* inp);
void newline();

int main(int argc, char* argv[]) {

	int i, opt, var, verbose, quiet;
	int coefs_only, intervals_only;

	FTYPE gamma0, rho0, P0, U0;	//input vars
	FTYPE gamma3, rho3, P3, U3;
	FTYPE C3;

	FTYPE a[7];	//coefficients

	FTYPE X, alpha0, alpha3, e0, e3, C0;	//intermediate vars
	FTYPE n, mu, nu, Z, Y, A, B, step;
	FTYPE tmp1, tmp2;
	int roots_found, total_pos_roots, total_neg_roots, segments;

	interval roots_int_pos, roots_int_neg;
	interval tmp_int;
	interval* root_int = NULL;

	var = 1;	//default variant
	verbose = 0;	//verboseging off by default
	quiet = 0;	//prompt user for input
	coefs_only = 0;	//don't stop on coefs
	intervals_only = 0;	//don't stop on intervals

	roots_found = 0;
	total_pos_roots = 0;
	total_neg_roots = 0;
	segments = 10000;

	while((opt = getopt(argc, argv, "12vhqcis:")) != -1) {
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
	else {
		rho3 = gamma3*P3/sq(C3);
		alpha0 = (gamma0 + 1)/(gamma0 - 1);
		n = 2*gamma3/(gamma3 - 1);
		mu = (U3 - U0)*sqrt((gamma0 - 1)*rho0/(2*P0));
		nu = 2/(gamma3 - 1)*sqrt(gamma3*(gamma0 - 1)/2 * P3/P0 * rho0/rho3);
		X = P3/P0;
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
	else {
		a[0] = sq(X);	//Z^2n

		a[1] = -alpha0*sq(nu)*X;	//Z^(n+2)

		a[2] = 2*alpha0*nu*(mu + nu)*X;	//Z^(n + 1)

		a[3] = -(2 + sq(nu + mu)*alpha0)*X;	//Z^n

		a[4] = -sq(nu);	//Z^2

		a[5] = 2*nu*(mu + nu);	//Z

		a[6] = -sq(mu + nu) + 1;	//1
	}

	if(verbose | coefs_only) {	//print coefs
		for(i = 0; i < 7; i++)
			printf("a[%i] = %e\n", i, a[i]);
	}
	if(coefs_only)
		return(0);


	n = roundf(n);	//because it's integer anyway

	if(verbose && (var == 2)) {
		newline();
		printf("n = %f\n", n);
	}

	//initial localization guess
	A = max_abs(&(a[1]), 6);
	B = max_abs(a, 6);

	roots_int_pos.l = fabs(a[6])/(fabs(a[6]) + B);	//initial guess
	roots_int_pos.r = 1 + A/fabs(a[0]);

	roots_int_neg.l = -roots_int_pos.r;
	roots_int_neg.r = -roots_int_pos.l;

	//estimate amount of roots
	for(i = 0; i < 6; i++) {
		if(a[i]*a[i+1] < 0)
			total_pos_roots++;
	}

	if(var == 1) {
		for(i = 0; i < 6; i++) {
			if(a[i]*a[i + 1] > 0)
				total_neg_roots++;
		}
	}
	else {
		tmp1 = a[0];
		tmp2 = a[1];
		for(i = 0; i < 6; i++) {
			switch(i) {
			case 0:
				tmp2 *= pow(-1, n + 2);
				break;
			case 1:
				tmp2 *= pow(-1, n + 1);
				break;
			case 2:
				tmp2 *= pow(-1, n);
				break;
			case 3:
				break;
			case 4:
				tmp2 *= -1;
				break;
			case 6:
				break;
			default:
				break;
			}

			if(tmp1*tmp2 < 0)
				total_neg_roots++;

			tmp1 = tmp2;
			if(i != 5)
				tmp2 = a[i+2];
		}
	}

	if(verbose) {
		newline();
		printf("A = %e\n", A);
		printf("B = %e\n", B);
	}
	if(verbose) {
		newline();
		printf("Initial positive interval: ");
		print_int(&roots_int_pos);
		newline();
		printf("Values:");
		if(var == 1) {
			printf("[ %e; %e ]\n", fval_1(roots_int_pos.l, a), fval_1(roots_int_pos.r, a));
		}
		else {
			printf("[ %e; %e ]\n", fval_2(roots_int_pos.l, a, n), fval_2(roots_int_pos.r, a, n));
		}
		printf("No more than %i positive roots\n", total_pos_roots);

		newline();
		printf("Initial negative interval: ");
		print_int(&roots_int_neg);
		newline();
		printf("Values:");
		if(var == 1) {
			printf("[ %e; %e ]\n", fval_1(roots_int_neg.l, a), fval_1(roots_int_neg.r, a));
		}
		else {
			printf("[ %e; %e ]\n", fval_2(roots_int_neg.l, a, n), fval_2(roots_int_neg.r, a, n));
		}
		printf("No more than %i negative roots\n", total_neg_roots);
	}




	//find negative root intervals
	step = (roots_int_neg.r - roots_int_neg.l)/segments;
	tmp_int.l = roots_int_neg.l;
	tmp_int.r = roots_int_neg.l + step;

	for(i = 0; i < segments; i++) {
		if(var == 1) {
			tmp1 = fval_1(tmp_int.l, a)*fval_1(tmp_int.r, a);
		}
		else {
			tmp1 = fval_2(tmp_int.l, a, n)*fval_2(tmp_int.r, a, n);
		}

		if(tmp1 < 0) {	//if sign changes, save current interval
			roots_found++;
			root_int = (interval*)realloc((void*)root_int, roots_found*sizeof(interval));
			root_int[roots_found - 1].l = tmp_int.l;
			root_int[roots_found - 1].r = tmp_int.r;
		}

		tmp_int.l += step;
		tmp_int.r += step;
	}



	//find positive root intervals
	step = (roots_int_pos.r - roots_int_pos.l)/segments;
	tmp_int.l = roots_int_pos.l;
	tmp_int.r = roots_int_pos.l + step;

	for(i = 0; i < segments; i++) {
		if(var == 1) {
			tmp1 = fval_1(tmp_int.l, a)*fval_1(tmp_int.r, a);
		}
		else {
			tmp1 = fval_2(tmp_int.l, a, n)*fval_2(tmp_int.r, a, n);
		}
		
		if(tmp1 < 0) {	//if sign changes, save current interval
			roots_found++;
			root_int = (interval*)realloc((void*)root_int, roots_found*sizeof(interval));
			root_int[roots_found - 1].l = tmp_int.l;
			root_int[roots_found - 1].r = tmp_int.r;
		}

		tmp_int.l += step;
		tmp_int.r += step;
	}

	if(verbose) {
		newline();
		printf("%i root intervals found:\n", roots_found);
	}
	if(verbose | intervals_only) {
		for(i = 0; i < roots_found; i++) {
			print_int(&(root_int[i]));
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

FTYPE sq(FTYPE x) {
	return pow(x, 2);
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

FTYPE max_abs(FTYPE* arr, int length) {
	int i;
	FTYPE ret;

	ret = (FTYPE)fabs(*arr);

	for(i = 0; i < length; i++) {
		if( fabs(*(arr + i)) > ret ) {
			ret = fabs(*(arr + i));
		}
	}

	return ret;
}

FTYPE fval_1(FTYPE x, FTYPE* coef) {
	int i;
	FTYPE res = 0;

	for(i = 0; i < 7; i++) {
		res += coef[i]*pow(x, 6 - i);
	}

	return res;
}

FTYPE fval_2(FTYPE x, FTYPE* coef, FTYPE n) {
	int i;
	FTYPE res = 0;

	for(i = 0; i < 7; i++) {
		switch(i) {
		case 0:
			res += coef[i]*pow(x, 2*n);
			break;

		case 1:
			res += coef[i]*pow(x, n + 2);
			break;

		case 2:
			res += coef[i]*pow(x, n + 1);
			break;

		case 3:
			res += coef[i]*pow(x, n);
			break;

		case 4:
			res += coef[i]*pow(x, 2);
			break;

		case 5:
			res += coef[i]*x;	
			break;

		case 6:
			res += coef[i];
			break;

		default:
			printf("Misterious error occured\n");
			exit(1);
		}
	}

	return res;
}

void print_int(interval* inp) {
	printf("[ %e; %e ]", (*inp).l, (*inp).r);
	return;
}

void newline() {
	printf("\n");
}

FTYPE find_root_dichotomy();
