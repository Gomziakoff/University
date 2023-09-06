#include <iostream>
#include <omp.h>
#include "functions.h"

using namespace std;

int main()
{
	srand(time(NULL));
	double alf, alf_min = 0.0000001, C = 0, b = 5;
	double a[1000][3]{};
	cout << "Введите значение шага: ";
	cin >> alf;
	while (C < 0.75 || C > 0.99) {
		cout << "\nВведите параметр затухания [0.75,0.99]: ";
		cin >> C;
		if (C < 0.75 || C > 0.99) cout << "Error";
	}
#pragma omp parallel for
	{
		for (int i = 0; i < 1000; i++) {
			a[i][1] = frand(-1000, 1000);
			minim(b, alf, alf_min, C, a[i][0], a[i][1], a[i][2]);
		}
	}
	double min;
	min = a[0][0];
	for (int i = 0; i < 1000; i++) {
		if (a[i][0] < min) min = a[i][0];
	}
	cout << "Минимум: " << min;
}
	

