#include "iostream"
#include "string"

using namespace std;

class TCat {
private:
	string name;
public:
	TCat(const string& _name) {
		name = _name;
	}
	void say() {
		cout << "my name is " << name << endl;
	}
};

int main() {
	TCat c("qaw");

	c.say();
}
