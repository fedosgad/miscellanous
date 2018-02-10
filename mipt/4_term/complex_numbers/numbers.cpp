#include "iostream"

using namespace std;

class TCmplx {
private:

	int real_part;
	int imag_part;


protected:

public:

	TCmplx(const int& _Re, const int& _Im) {		//constructor, takes real and imaginary part (as pointers, not copies)
		real_part = _Re;
		imag_part = _Im;
	}

	int Re() {
		return(real_part);
	}

	int Im() {
		return(imag_part);
	}

};


int main() {
	TCmplx z1(1, 2);
	TCmplx z2(3, 4);

	cout << z1.Re() << " " << z2.Im() << endl;

	return 0;
}
