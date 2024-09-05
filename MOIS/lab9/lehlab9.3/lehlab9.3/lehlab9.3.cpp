#include <iostream>
#include <vector>
#include <ctime>

using namespace std;

bool gen(vector<int>& a, int n) {
	int j = a.size() - 2;
	while (j != -1 && a[j] > a[j + 1]) j--;
	if (j == -1) return false;
	int k = n - 1;
	while (a[k] < a[j]) k--;
	swap(a[k], a[j]);
	int l = j + 1;
	int r = n - 1;
	while (l < r) {
		swap(a[l], a[r]);
		l++;
		r--;
	}
	return true;
}

void print(vector<int> a) {
	//static int num = 1;
	cout.width(3);
	//cout << num++ << ": ";
	for (int i = 0; i < a.size(); i++)
		cout << a[i] << " ";
	cout << endl;
}

void hasTwoFixed(vector<int> a) {
	for (int i = 0; i < a.size(); i++) {
		if (a[i] == i + 1) {
			for (int j = i + 1; j < a.size(); j++) {
				if (a[j] == j + 1) {
					print(a);
					return;
				}
			}
		}
	}
}

int main() {

	vector<int> a;
	int time = 0;
	for (int n = 4; time < 1; n++) {

		unsigned int start = clock();
		for (int i = 1; i <= n; i++) {
			a.push_back(i);
		}
		//print(a, n);
		hasTwoFixed(a);
		while (gen(a, n)) {
			//print(a, n);
			hasTwoFixed(a);
		}
		unsigned int end = clock();
		time = (end - start) / 1000;
		cout << endl << "n = " << n << " time = " << time << "s.\n";
		a.clear();
	}
}