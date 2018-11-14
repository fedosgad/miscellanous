#include <iostream>
#include <vector>
#include <utility>
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

	FTYPE get_coef(int pow) {
		return coef[n - pow];
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

class spline_segment {
private:
	polynomial poly;

	FTYPE border_l;
	FTYPE border_r;

public:
	spline_segment() {}

	spline_segment(polynomial _poly, FTYPE _border_l, FTYPE _border_r) {
		poly = _poly;
		border_l = _border_l;
		border_r = _border_r;
	}

	FTYPE get_border_l() {
		return border_l;
	}

	FTYPE get_border_r() {
		return border_r;
	}

	FTYPE val(FTYPE x) {
		return poly.val(x);
	}

};

int main() {
	FTYPE x;
	int n;

	vector< pair< FTYPE, FTYPE > > data;	//initial data
	vector< spline_segment > spline;	//resulting cubic spline

	cout << "Amount of values: ";
	cin >> n;

	cout << "(x;y) pairs (space-separated x and y)" << endl;

	for(int i = 0; i < n; i++) {	//enter (x;y) pairs
		FTYPE tmp_x, tmp_y;
		cin >> tmp_x >> tmp_y;
		data.push_back( make_pair(tmp_x, tmp_y) );
	}

	polynomial newton_poly(n);

	for(int i = 0; i < n; i++) {	//calculate Newton's poly's coefs
		
	}

	return 0;
}
