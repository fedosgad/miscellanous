#include "iostream"
#include "vector"
#include "list"
#include "iostream"
#include "string"

using namespace std;

/*class implementing hash table with chaining
 * methods:
 * 	set(key, val)
 * 	get(key)
 * 	size()
 * 	::iterator	difficult one
 */

template<typename TKEY, typename TVAL>
class THashtable {		//from int to int

private:

	vector<list<pair<TKEY, TVAL> > > storage;

	int hash(int key) {
		return 0;
	}

	int hash(string key) {
		return 0;
	}

public:

	THashtable() {
		storage.resize(1000);
	}

	THashtable(int size) {
		storage.resize(size);
	}

	void set(TKEY key, TVAL val) {
		storage[hash(key)].push_back({key, val});
	}

	int get(TKEY key) {

		//~ if(storage)		//TODO: check for empty table

		int addr = hash(key)%storage.size();

		for(auto& i : storage[addr] ) {
			if(i.first == key)
				return i.second;
		}

		//~ if()		//TODO: check for "nothing found"
	}

};


int main() {

	int inp;
	THashtable<int, int> a;

	cin >> inp;
	a.set(1, inp);

	cin >> inp;
	a.set(2, inp);

	cout << a.get(1) << " " << a.get(2) << endl;

}
