#include <iostream>
#include <vector>
#include <utility>
#include <cmath>

#define FTYPE float

using namespace std;

FTYPE f(pair< FTYPE, FTYPE > a, pair< FTYPE, FTYPE > b);

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
	FTYPE x, tmp, tmp_coef;
	int n;

	vector< pair< FTYPE, FTYPE > > data;	//initial data
	vector< spline_segment > spline;	//resulting cubic spline
	vector< int > counter;	//cycle counters for Newton poly's calculation
	vector< FTYPE > div_diff;
	vector< FTYPE > b_coef;

	cout << "Amount of values: ";
	cin >> n;

	cout << "(x;y) pairs (space-separated x and y)" << endl;

	for(int i = 0; i < n; i++) {	//enter (x;y) pairs
		FTYPE tmp_x, tmp_y;
		cin >> tmp_x >> tmp_y;
		data.push_back( make_pair(tmp_x, tmp_y) );
	}

	polynomial newton_poly(n);

	counter.resize(n);
	b_coef.resize(n);
	div_diff.resize(n);

	b_coef[0] = data[0].second;

	for(int i = 0; i < n - 1; i++) {	//calculate divided diffs
		if(i == 0) {
			for(int j = 1; j < n - i; j++) {
				div_diff[j - 1] = f(data[j - 1], data[j]);
			}
		}
		else {
			for(int j = 1; j < n - i; j++) {
				div_diff[j - 1] = f( make_pair( data[j - 1].first, div_diff[j - 1] ), 
							make_pair( data[j + i].first, div_diff[j - 1] )
							);
			}
		}

		b_coef[i + 1] = div_diff[0];

	}

	cout << "b_coefs:" << endl;
	for(int i = 0; i < n - 1; i++)
		cout << b_coef[i] << endl;
	cout << endl;

	int cur_count = n - 1;
	tmp = 0;
	tmp_coef = 0;

	for(int i = 0; i < n; i++) {	//calculate poly's coefs
		for(int j = 0; j < n - i; j++)
				counter[j] = j;	//initial fill-in

		bool flag = false;

		tmp = data[ counter[0] ].first;
		for(int k = 1; k < n - i; k++)
			tmp *= data[ counter[k] ].first;
		tmp_coef += tmp;

		cur_count = n - 1 - i;

		while( !flag ) {
			while(1) {	//generate next combination
				if(counter[cur_count] < i + cur_count) {
					counter[cur_count]++;
					for(int l = cur_count + 1; l < n - i; l++)
						counter[l] = counter[l - 1] + 1;

					cur_count = n - 1;
					break;
				}
				else if((cur_count == 0) && (counter[0] == i)) {
					flag = true;
					break;
				}
				else {
					cur_count--;
				}
			}

			tmp = data[ counter[0] ].first;
			for(int k = 1; k < n - i; k++)
				tmp *= data[ counter[k] ].first;
			tmp_coef += tmp;
		}

		newton_poly.set_coef(i, tmp_coef*b_coef[i]);

		cout << tmp_coef << endl;

		tmp_coef = 0;

	}

	return 0;
}

FTYPE f(pair< FTYPE, FTYPE > a, pair< FTYPE, FTYPE > b) {
	return (b.second - a.second)/(b.first - a.first);
}

