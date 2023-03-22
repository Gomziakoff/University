#include <iostream>

using namespace std;

int binsearch(int* a, int n) {
	int target;
	cin >> target;
	int left = 0, right = n - 1;
	if (a[left] > target) {
		return 0;
	}
	if (a[right] < target) {
		return right + 1;
	}
	while (true) {
		int middle = (left + right) / 2;
		int found = a[middle];
		if (found == target) {
			return middle;
		}
		if (middle == left) {
			if (a[right] == target) {
				return right;
			}
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
	int target;
	cin >> target;
	int found = -1, low = 0, high = n - 1;
	while (a[low]<target && a[high]>target) {
		int mid = low + (target - a[low]) * (high - low) / (a[high] - a[low]);
		if (a[mid] < target) {
			low = mid++;
		}
		else if (a[mid]>target) {
			high = mid - 1;
		}
		else {
			return mid;
		}
	}
	if (a[low] = target) {
		return low;
	}
	if (a[high] < target) {
		return high;
	}
	else {
		return -1;
	}
}

void merge(int* a, int n)
{
	int mid = n / 2;
	if (n % 2 == 1)
		mid++;
	int h = 1;
	int* c = new int[n];
	int step;
	while (h < n)
	{
		step = h;
		int i = 0;
		int j = mid;
		int k = 0;
		while (step <= mid)
		{
			while ((i < step) && (j < n) && (j < (mid + step)))
			{
				if (a[i] < a[j])
				{
					c[k] = a[i];
					i++; k++;
				}
				else {
					c[k] = a[j];
					j++; k++;
				}
			}
			while (i < step)
			{ 
				c[k] = a[i];
				i++; k++;
			}
			while ((j < (mid + step)) && (j < n))
			{ 
				c[k] = a[j];
				j++; k++;
			}
			step = step + h; 
		}
		h = h * 2;
		for (i = 0; i < n; i++)
			a[i] = c[i];
	}
}

int main() {
	int n, target;
	cin >> n;
	int* a = new int[n];
	for (int i = 0; i < n; i++) {
		cin >> a[i];
	}
	merge(a, n);
	int choise;
	cin >> choise;
	switch (choise)
	{
	case 1:
		binsearch(a, n);
		break;
	case 2:
		interpol(a, n);
		break;
	}
	


}