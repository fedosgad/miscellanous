#include <stdio.h>
#include <math.h>

float fval(float x);

int main() {

	int i;
	float l, r, step, x;

	printf("Enter interval data:\n");
	printf("Xн = ");
	scanf("%f", &l);
	printf("Xп = ");
	scanf("%f", &r);

	printf("Enter dx:\n");
	printf("dx = ");
	scanf("%f", &step);

	printf("\n");

	printf("x\tf(x)\n");

	x = l;
	
	while(x <= r) {
		printf("%.1f\t%f\n", x, fval(x));
		x += step;
	}

	//Here should be check for case (l - r)/step isn't integer
	//But there isn't one

	return 0;
}

float fval(float x) {
	return(log(pow(x, 2)) + pow(x, 2) + 2);
}

