#include <iostream>
#include <vector>
#include <ctime>

using namespace std;

bool gen(vector<int>& a, int n) {
	int j = a.size()-2;
	while (j != -1 && a[j] > a[j + 1]) j--;
	if (j == -1) return false;
	int k = n-1;
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

void print(vector<int> a,int n) {
	static int num = 1; 
	cout.width(3);
	cout << num++ << ": ";
	for (int i = 0; i < n; i++)
		cout << a[i] << " ";
	cout << endl;
}


int main() {

	vector<int> a;
	int time = 0;
	for (int n = 4; time < 120; n++) {

		unsigned int start = clock();
		for (int i = 1; i <= n; i++) {
			a.push_back(i);
		}
		//print(a, n);
		while (gen(a, n)) {
			//print(a, n);
		}
		unsigned int end = clock();
		time = (end - start)/1000;
		cout << endl << "n = "<<n<<" time = " << time << "s.\n";
		a.clear();
	}
}