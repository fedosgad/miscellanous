#include "vector"
#include "iostream"

using namespace std;

int main() {
	vector<int> v;

	v.push_back(1);
	v.push_back(200);
	v.pop_back();
	v.push_back(3);

	cout << v[1] << endl;
	cout << v.size() << endl;

	cout << "Elements:" << endl;

	for(auto& i : v) {		//
		cout << i << endl;
	}

	cout << "Again" << endl;

	for(int i = 0; i < v.size(); i++) {
		cout << v[i] << endl;
	}

	cout << "And again" << endl;

	for(vector<int>::iterator i = v.begin(); i != v.end(); i++) {
		cout << *i << endl;
	}

}
