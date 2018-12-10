#include <stdio.h>
#include <math.h>

#define FTYPE float

#define LEFT_BORDER 1.0
#define RIGHT_BORDER 2.0
#define EPSILON 1e-4
#define Y_LB 1.0	//Y at left border
#define K 4
#define H 0.5

FTYPE f(FTYPE x, FTYPE y);

int main() {
	FTYPE h, cur_x, cur_y;
	FTYPE f1, f2, f3, f4;
	FTYPE step;
	FTYPE diff;

	FTYPE cur_y_vals[11];
	FTYPE prev_y_vals[11];

	int i, first_time, iteration, flag;

	step = (RIGHT_BORDER - LEFT_BORDER)/10;
	h = H;
	diff = 0;

	first_time = 1;
	flag = 0;
	iteration = 0;

	while(!flag) {
		printf("Iteration %i, h = %e, ", iteration, h);

		if(!first_time) {	//save previous results
			for(i = 0; i < 11; i++)
				prev_y_vals[i] = cur_y_vals[i];
		}

		cur_y_vals[0] = Y_LB;
		cur_y = Y_LB;
		cur_x = LEFT_BORDER;

		while(cur_x < RIGHT_BORDER) {	//obtain values
			f1 = f(cur_x, cur_y);	//calculate next Yn
			f2 = f(cur_x + h/2, cur_y + h/2*f1);
			f3 = f(cur_x + h/2, cur_y + h/2*f2);
			f4 = f(cur_x + h, cur_y + h*f3);

			cur_x += h;
			cur_y = cur_y + h/6*(f1 + 2*f2 + 2*f3 + f4);

			for(i = 1; i < 11; i++) {	//save value if needed
				if(fabs(cur_x - (LEFT_BORDER + i*step)) < h/2) {
					cur_y_vals[i] = cur_y;
					break;
				}
			}
		}

		if(!first_time) {	//check if enough
			diff = fabs(prev_y_vals[0] - cur_y_vals[0]);
			for(i = 0; i < 11; i++) {
				if(fabs(prev_y_vals[i] - cur_y_vals[i]) > diff)
					diff = fabs(prev_y_vals[i] - cur_y_vals[i]);
			}

			if(diff/(pow(2, K) - 1) < EPSILON)
				flag = 1;
		}

		printf("diff = %e\n", diff);

		first_time = 0;
		iteration++;
		h = h/2;
	}

	printf("\n");

	printf("x:");
	for(i = 0; i < 11; i++)
		printf("\t%e", LEFT_BORDER + i*step);
	printf("\n");

	printf("y(2h):");
	for(i = 0; i < 11; i++)
		printf("\t%e", prev_y_vals[i]);
	printf("\n");

	printf("y(h):");
	for(i = 0; i < 11; i++)
		printf("\t%e", cur_y_vals[i]);
	printf("\n");

	printf("dy:");
	for(i = 0; i < 11; i++)
		printf("\t%e", fabs(prev_y_vals[i] - cur_y_vals[i]));
	printf("\n");

	return 0;
}

FTYPE f(FTYPE x, FTYPE y) {
	return (2*pow(x, 3) + pow(x, 2) - pow(y, 2))/(2*pow(x, 2)*y);
}

