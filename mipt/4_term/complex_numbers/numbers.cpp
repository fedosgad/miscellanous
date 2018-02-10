#include "iostream"

using namespace std;

class TCmplx {

friend ostream& operator<<(ostream& o, const TCmplx& i);

private:

	int real_part;
	int imag_part;


protected:

public:

	TCmplx(const int& _Re, const int& _Im) {		//constructor, takes real and imaginary part (as pointers, not copies)
		real_part = _Re;
		imag_part = _Im;
	}

	TCmplx() {
	};

	int Re() const {
		return(real_part);
	}

	int Im() const {
		return(imag_part);
	}

	TCmplx operator+(TCmplx& b) {
		TCmplx sum;

		sum.real_part = real_part + b.real_part;
		sum.imag_part = imag_part + b.imag_part;

		return sum;
	}

	TCmplx operator-(TCmplx& b) {
		TCmplx diff;

		diff.real_part = real_part - b.real_part;
		diff.imag_part = imag_part - b.imag_part;

		return diff;
	}

	TCmplx operator -() {
		TCmplx neg;

		neg.real_part = -real_part;
		neg.imag_part = -imag_part;

		return neg;
	}

	TCmplx conj() {
		TCmplx conj;

		conj.real_part = real_part;
		conj.imag_part = -imag_part;

		return conj;
	}

	

};


ostream& operator<<(ostream& o, const TCmplx& i) {
	o << i.Re() << "+i*(" << i.Im() << ")";
	return o;
}

int main() {
	TCmplx z1(1, 2);
	TCmplx z2(3, 4);

	cout << z1.Re() << " " << z2.Im() << endl;

	TCmplx z3 = z1 - z2;
	cout << z3.Re() << " " << z3.Im() << endl;

	z3 = z3.conj();
	cout << z3.Re() << " " << z3.Im() << endl;

	cout << z3 << endl;

	cout << -z3 << endl;

	return 0;
}
