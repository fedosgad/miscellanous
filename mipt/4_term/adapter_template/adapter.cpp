#include <iostream>

#include "american.hpp"

using namespace std;

class TTherm {
public:
	TTherm() {
	}

	virtual int GetThermCelsius() = 0;
};

class THgTherm : public TTherm {
public:
	THgTherm() {
	}

	int GetThermCelsius() override {
		return 40;
	}
};


class TAmericanAdapter : public TTherm {
private:
	TAmerican *therm;

public:
	TAmericanAdapter() {
		therm = new TAmerican;
	}

	int GetThermCelsius() override {
		int temp;
		temp = (therm->GetThermFahr() - 32) * 5 / 9;
		return temp;
	}
};


int main() {
	THgTherm t_therm;

	cout << t_therm.GetThermCelsius() << endl;

	TAmerican a_therm;

	cout << a_therm.GetThermFahr() << endl;

	TAmericanAdapter adapted_therm;

	cout << adapted_therm.GetThermCelsius() << endl;
}
