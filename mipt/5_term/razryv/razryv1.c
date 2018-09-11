#include <stdio.h>
#include <math.h>

#define FTYPE float //change this to double for greater precesion
#define PRINT_VALUES() printf("\n%f\n%f\n%f\n%f\n%f\n%f\n%f\n%f\n", \
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
FTYPE input_fract(char* var_name);
FTYPE input_exp(char* var_name);


int main(int argc, char* argv[]) {

	int i;

	FTYPE gamma0, rho0, P0, U0;
	FTYPE gamma3, rho3, P3, U3;
	FTYPE a[7];	//coefficients

	usage();

	//time to enter values
	gamma0 = input_fract("gamma0");
	rho0 = input_exp("rho0");
	U0 = input_exp("U0");
	P0 = input_exp("P0");

	gamma3 = input_fract("gamma3");
	rho3 = input_exp("rho3");
	U3 = input_exp("U3");
	P3 = input_exp("P3");

	//~ PRINT_VALUES();


	FTYPE X, alpha0, alpha3, e0, e3, C0, C3;	//intermediate vars

	C0 = sqrt(gamma0*P0/rho0);
	C3 = sqrt(gamma3*P3/rho3);

	X = P3/P0;
	alpha0 = (gamma0 + 1)/(gamma0 - 1);
	alpha3 = (gamma3 + 1)/(gamma3 - 1);

	e0 = 2*sq(C0)/(gamma0*(gamma0 - 1)*sq(U3 - U0));
	e3 = 2*sq(C3)/(gamma3*(gamma3 - 1)*sq(U3 - U0));


	//calculating coefs
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


	printf("\n");
	for(i = 0; i < 7; i++)
		printf("a[%i] = %f\n", i, a[i]);


	return 0;
}

void usage() {
	printf(
		"Enter data. Format:\n"
		"a/b for gamma (for example, \"5/3\"),\n"
		"a.bEc for everything else (for example 1.2E3).\n"
	);
}

FTYPE sq(FTYPE x) {
	return pow(x, 2);
} 

FTYPE input_fract(char* var_name) {

	FTYPE num, denom;

	printf("%s = ", var_name);
	scanf("%f/%f", &num, &denom);

	return num/denom;
}

FTYPE input_exp(char* var_name) {

	FTYPE mant, exp;

	printf("%s = ", var_name);
	//~ scanf("%fE%f", &mant, &exp);
	scanf("%f", &mant);

	return mant;
}
