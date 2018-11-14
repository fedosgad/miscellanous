#include <iostream>
#include <vector>
#include <cmath>

#define FTYPE float

using namespace std;

class polynomial {
private:
	int n;	//polynome degree
	vector<FTYPE> coef;	//coefficients, highest power first

public:
	polynomial() {}

	polynomial(const int _n) {
		n = _n;
		coef.resize(n + 1);
	}

	FTYPE val(FTYPE x) {
		FTYPE result = 0;
		for(int i = 0; i <= n; i++) {
			result += coef[i] * pow(x, n - i);
		}
		return result;
	}

	void set_coef(int pow, FTYPE val) {
		coef[n - pow] = val;
	}

	int get_n() {
		return n;
	}

	polynomial deriv() {
		polynomial result(n - 1);

		for(int i = 0; i < n; i++)
			result.set_coef(i, coef[i + 1]*(i + 1));

		return result;
	}

};

int main() {
	FTYPE x;
	polynomial poly(2);

	poly.set_coef(0, 1);
	poly.set_coef(1, 1);
	poly.set_coef(2, 1);

	polynomial der_poly = poly.deriv();

	cin >> x;

	cout << poly.val(x) << endl;
	cout << der_poly.val(x) << endl;

	return 0;
}
