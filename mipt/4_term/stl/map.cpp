#include "iostream"
#include "map"
#include "vector"
#include "string"

using namespace std;

int main() {

	map<int, string> m = {{0, "zero"}, {2, "two"}};

	m[1] = "one";
	m[100] = "hundred";
	m[5] = "five";

	for(auto i = m.begin(); i != m.end(); i++) {
		cout << i->first << " " << i->second << endl;
	}

	cout << endl;

	for(auto i : m) {
		cout << i.first << " " << i.second << endl;
	}

	pair<int, int> p;
	p.first = 1;
	p.second = 4;

	cout << endl;

	cout << p.first << " " << p.second << endl;
}
