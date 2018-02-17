#include "iostream"
#include "cmath"

using namespace std;

class TComplex {

friend ostream& operator<<(ostream& o, const TComplex& i);

private:

	float real_part;
	float imag_part;


protected:

public:

	TComplex(const float& _Re, const float& _Im) {		//constructor, takes real and imaginary part (as pointers, not copies)
		real_part = _Re;
		imag_part = _Im;
	}

	TComplex() {
	};

	float Re() const {
		return(real_part);
	}

	float Im() const {
		return(imag_part);
	}

	TComplex operator+(TComplex& b) {
		TComplex sum;

		sum.real_part = real_part + b.real_part;
		sum.imag_part = imag_part + b.imag_part;

		return sum;
	}

	TComplex operator-(TComplex& b) {
		TComplex diff;

		diff.real_part = real_part - b.real_part;
		diff.imag_part = imag_part - b.imag_part;

		return diff;
	}

	TComplex operator-() {
		TComplex neg;

		neg.real_part = -real_part;
		neg.imag_part = -imag_part;

		return neg;
	}

	TComplex conj() {
		TComplex conj;

		conj.real_part = real_part;
		conj.imag_part = -imag_part;

		return conj;
	}

	TComplex operator*(TComplex& b) {
		TComplex mul;

		mul.real_part = real_part * b.real_part - imag_part * b.imag_part;
		mul.imag_part = imag_part * b.real_part + real_part * b.imag_part;

		return mul;
	}

	TComplex operator/(TComplex& b) {
		TComplex div;

		div.real_part = (real_part*b.real_part + imag_part*b.imag_part)/(b.real_part*b.real_part + b.imag_part*b.imag_part);
		div.imag_part = (imag_part*b.real_part - real_part*b.imag_part)/(b.real_part*b.real_part + b.imag_part*b.imag_part);

		return div;
	}

	float abs() {
		return(sqrt(real_part*real_part + imag_part*imag_part));
	}

	float arg() {
		float arg;

		if(real_part > 0) {
			arg = atan(imag_part/real_part);
		}
		else if(real_part < 0) {
			if(imag_part >= 0) {
				arg = atan(imag_part/real_part) + M_PI;
			}
			else {
				arg = atan(imag_part/real_part) - M_PI;
			}
		}
		else {
			if(imag_part > 0) {
				arg = M_PI/2;
			}
			else if(imag_part < 0) {
				arg = -M_PI/2;
			}
			else {
				arg = NAN;
			}
		}
	}
};


ostream& operator<<(ostream& o, const TComplex& i) {
	o << i.Re() << " + " << i.Im() << "i";
	return o;
}

int main() {
	TComplex z1(1, 2);
	TComplex z2(3, 4);

	cout << z1.Re() << " " << z2.Im() << endl;

	TComplex z3 = z1 - z2;
	cout << z3 << endl;

	z3 = z3.conj();
	cout << z3 << endl;

	cout << -z3 << endl;

	TComplex z4(1, 1);

	cout << z4.abs() << " " << z4.arg() << endl;

	z1 = z4.conj();
	z1 = z1 * z4;
	cout << z1.abs() << " " << z1.arg() << endl;

	TComplex z5 = z2/z4;
	cout << z5 << endl;

	return 0;
}
