#include <iostream>
#include <cmath>
#include <iomanip>
#include "f.h"

using namespace std;

void raboch(double *rabraspred, int* rab, int rabs) {
	for (int i = 0; i < 5; i++) {
		rab[i] = rabraspred[i] * rabs;
	}
}

int process(int* rab) {
	int day = 0; int complete = 0;
	while (complete < 100) {
		for (int i = 0; i < rab[0]; i++) {
			complete += U(0.2, 0.5) * N(6, 0.5);
		}
		for (int i = 0; i < rab[1]; i++) {
			complete += U(0.1, 0.3) * N(5, 0.5);
		}
		for (int i = 0; i < rab[2]; i++) {
			complete += U(0.05, 0.02) * N(4, 0.5);
		}
		for (int i = 0; i < rab[3]; i++) {
			complete += U(0.01, 0.02) * N(2, 0.5);
		}
		for (int i = 0; i < rab[4]; i++) {
			complete += U(0.3, 0.75) * N(1, 0.5);
		}
		day++;
	}
	return day;
}

int main() {
	srand(time(NULL));
	int rabs = 10;
	double rabraspred[5] = { 0.6,0.1,0.1,0.2,0.0 };
	int rab[5]={0};
	raboch(rabraspred, rab, rabs);
	int days[100],min=999,max=0,sred=0;
	for (int i = 0; i < 100; i++) {
		days[i] = process(rab);
		if (min > days[i]) min = days[i];
		if (max < days[i]) max = days[i];
		sred += days[i];
		cout << "days: " << days[i]<<endl;
	}
	cout << "min days: " << min << "\nmax days: " << max << "\naverage days: " << sred / 100;
	
}