#include <iostream>
#include <vector>
#include <cmath>

#define FTYPE float

using namespace std;

class polynomial {
private:
	int n;	//polynome degree
	vector<FTYPE> coef;	//coefficients, power highest power first

public:
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


};

int main() {
	FTYPE x;
	polynomial poly(0);

	poly.set_coef(0, 1.65);

	cin >> x;

	cout << poly.val(x) << endl;

	return 0;
}
