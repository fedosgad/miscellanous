#include <math.h>
#include <stdlib.h>

#include "calculations.h"

void set_var(int num) {
	var = num;
}

FTYPE fval_1(FTYPE x) {
	int i;
	FTYPE res = 0;

	for(i = 0; i < 7; i++) {
		res += a[i]*pow(x, 6 - i);
	}

	return res;
}

FTYPE fval_2(FTYPE x) {
	int i;
	FTYPE res = 0;

	for(i = 0; i < 7; i++) {
		switch(i) {
		case 0:
			res += a[i]*pow(x, 2*n);
			break;

		case 1:
			res += a[i]*pow(x, n + 2);
			break;

		case 2:
			res += a[i]*pow(x, n + 1);
			break;

		case 3:
			res += a[i]*pow(x, n);
			break;

		case 4:
			res += a[i]*pow(x, 2);
			break;

		case 5:
			res += a[i]*x;
			break;

		case 6:
			res += a[i];
			break;

		default:
			break;
		}
	}

	return res;
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

FTYPE sq(FTYPE x) {
	return pow(x, 2);
}

FTYPE find_root_dichotomy() {

}

void calc_interm_vars() {
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

		if(var == 2)
			n = roundf(n);
	}

}

void calc_coefs() {
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
}

void estimate_amount() {
	int i;
	FTYPE tmp1, tmp2;

	for(i = 0; i < 6; i++) {	//positive roots
		if(a[i]*a[i+1] < 0)
			total_pos_roots++;
	}

	if(var == 1) {	//negative roots
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

}

void initial_guess() {
	A = max_abs(&(a[1]), 6);
	B = max_abs(a, 6);

	roots_int_pos.l = fabs(a[6])/(fabs(a[6]) + B);	//initial guess
	roots_int_pos.r = 1 + A/fabs(a[0]);

	roots_int_neg.l = -roots_int_pos.r;
	roots_int_neg.r = -roots_int_pos.l;
}

void find_root_ints(int segments) {
	FTYPE step, tmp;
	interval tmp_int;
	int i;

	roots_found = 0;

	root_int = NULL;

	//find negative root intervals
	step = (roots_int_neg.r - roots_int_neg.l)/segments;
	tmp_int.l = roots_int_neg.l;
	tmp_int.r = roots_int_neg.l + step;

	for(i = 0; i < segments; i++) {
		if(var == 1) {
			tmp = fval_1(tmp_int.l)*fval_1(tmp_int.r);
		}
		else {
			tmp = fval_2(tmp_int.l)*fval_2(tmp_int.r);
		}

		if(tmp < 0) {	//if sign changes, save current interval
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
			tmp = fval_1(tmp_int.l)*fval_1(tmp_int.r);
		}
		else {
			tmp = fval_2(tmp_int.l)*fval_2(tmp_int.r);
		}
		
		if(tmp < 0) {	//if sign changes, save current interval
			roots_found++;
			root_int = (interval*)realloc((void*)root_int, roots_found*sizeof(interval));
			root_int[roots_found - 1].l = tmp_int.l;
			root_int[roots_found - 1].r = tmp_int.r;
		}

		tmp_int.l += step;
		tmp_int.r += step;
	}

}

//Getter functions

FTYPE get_a(int i) {
	return a[i];
}

FTYPE get_n() {
	return n;
}

FTYPE get_A() {
	return A;
}

FTYPE get_B() {
	return B;
}

interval get_roots_int(int type) {	//type == 1 -> pos. interval, else neg.
	if(type == 1)
		return roots_int_pos;
	else
		return roots_int_neg;
}

int get_roots_amount(int type) {	//same, but also type == 0 -> total (after they are found)
	switch(type) {
	case 0:
		return roots_found;
	case 1:
		return total_pos_roots;
	case 2:
		return total_neg_roots;
	default:
		break;
	}
}

