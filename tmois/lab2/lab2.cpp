#include <iostream>

using namespace std;

int binsearch(int* a, int n, int target) {
	int middle, found;
	int iter = 0;
	int left = 0, right = n - 1;
	if (a[left] > target) {
		cout << "bin iter =" << iter << endl;
		return 0;
	}
	if (a[right] < target) {
		cout << "bin iter =" << iter << endl;
		return right + 1;
	}
	while (true) {
		iter++;
		 middle = (left + right) / 2;
		 found = a[middle];
		if (found == target) {
			cout << "bin iter =" << iter << endl;
			return middle;
		}
		if (middle == left) {
			if (a[right] == target) {
				cout << "bin iter =" << iter << endl;
				return right;
			}
			cout << "bin iter =" << iter << endl;
			return middle + 1;
		}
		if (found < target) {
			left = middle;
		}
		else {
			right = middle;
		}
	}
}

int interpol(int* a, int n, int target) {
	int iter = 0;
	int found = -1, low = 0, high = n - 1;
	while (a[low]<target && a[high]>target) {
		iter++;
		int mid = low + (target - a[low]) * (high - low) / (a[high] - a[low]);
		if (a[mid] < target) {
			low = mid+1;
		}
		else if (a[mid] > target) {
			high = mid - 1;
		}
		else {
			cout << "interpolation iter =" << iter << endl;
			return mid;
		}
		if (a[low] = target) {
			cout << "interpolation iter =" << iter << endl;
			return low;
		}
		if (a[high] < target) {
			cout << "interpolation iter =" << iter << endl;
			return high;
		}
		else {
			return -1;
		}
	}
}



int main() {
	int n, target;
	cout << "inter number of elements\n";
	cin >> n;
	int* a = new int[n];
	cout << "enter numbers\n";
	for (int i = 0; i < n; i++) {
		cin >> a[i];
	}
	cout << "enter target element\n";
	cin >> target;
	int choise;
	cout << "enter choise\n";
	cin >> choise;
	switch (choise)
	{
	case 1:
		cout << binsearch(a, n, target);
		break;
	case 2:
		cout << interpol(a, n, target);
		break;
	case 3:
		cout << binsearch(a, n, target)<<endl;
		cout << interpol(a, n, target)<<endl;
		break;
	case 4:
		mergesort(a, 0, n);
		for (int i = 0; i < n; i++) {
			cout << a[i];
		}
		break;
	}
	


}