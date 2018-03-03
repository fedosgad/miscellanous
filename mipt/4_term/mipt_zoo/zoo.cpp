#include <iostream>
#include <string>
#include <map>

using namespace std;

/*класс Факультет
 * методы:
 * 	string say() - кричалка факультета
 * 	bool IsEnoughScore() - проходишь ли по баллам
 * требование - использовать шаблон "Фабрика" и exception'ы*/

class TDepartment {
protected:

	string motto;

	int MathThresh;	//минимальные баллы ЕГЭ
	int PhysThresh;
	int RusThresh;

public:

	TDepartment() {
		motto = "";
		MathThresh = 60;
		PhysThresh = 60;
		RusThresh = 60;
	}

	string say() {
		return motto;
	}

	virtual string name() {
		return("");
	}

	virtual bool IsEnoughScore(int math, int phys, int rus) {
		if(math + phys + rus >= MathThresh + PhysThresh + RusThresh)
			return(true);
		else
			return(false);
	}
};

class TDPQE : public TDepartment {	//ФФКЭ
public:

	TDPQE() {
		motto = "Вперед, Кванты! Гордость Физтеха!";
		MathThresh = 88;
		PhysThresh = 95;
		RusThresh = 88;
	}

	string name() override {
		return("DPQE");
	}

};

class TDASR : public TDepartment {	//ФАКИ
public:

	TDASR() {
		motto = "ФАКИ чемпион!";
		MathThresh = 87;
		PhysThresh = 92;
		RusThresh = 87;
	}

	string name() override {
		return("DASR");
	}
};

class TDGAP : public TDepartment {	//ФОПФ
public:

	TDGAP() {
		motto = "За Каменца!";
		MathThresh = 97;
		PhysThresh = 99;
		RusThresh = 94;
	}

	bool IsEnoughScore(int math, int phys, int rus) override {
		if ((math + phys + rus >= MathThresh + PhysThresh + RusThresh) &&
			(phys >= 90)
			)
			return(true);
		else
			return(false);
	}

	string name() override {
		return("DGAP");
	}

};


class UnknownDepartment : public exception {
public:
	string what() {
		return("Unknown department");
	}
};

TDepartment* GetDepartment(const string& type) {
	if((type == "DPQE") || (type == "dpqe")) {
		return new TDPQE;
	}
	else if((type == "DASR") || (type == "dasr")) {
		return new TDASR;
	}
	else if((type == "DGAP") || (type == "dgap")) {
		return new TDGAP;
	}
	else
		throw UnknownDepartment();
}

int main() {

	map <int, TDepartment> mipt;

	int math, phys, rus;

	mipt[1] = GetDepartment("dgap");
	mipt[2] = GetDepartment("dasr");
	mipt[4] = GetDepartment("dpqe");

	cout << "Welcome to MIPT! What are your scores?" << endl;
	cout << "Math: ";
	cin >> math;
	cout << "Physics: ";
	cin >> phys;
	cout << "Russian: ";
	cin >> rus;

	cout << "You can apply for the following departments:" << endl;

	for(int i = 0; i < 11; i++) {
		if(mipt[i]->IsEnoughScore(math, phys, rus))
			cout << mipt[i]->name() << endl;
	}

	return 0;
}
