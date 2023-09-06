#pragma once

#include <iostream>
#include <cmath>

using namespace std;

double f(double x, double b) {
	return cos(x) + (1 / b) * cos(7 * x + 1) + (1 / pow(b, 2)) * cos(49 * x + 2) + (1 / pow(b, 3)) * cos(343 * x + 3) + (1 / pow(b, 4)) * cos(2401 * x + 4);
	//return sin(x);
}

double frand(double fMin, double fMax)
{
	double f = (double)rand() / RAND_MAX;
	return fMin + f * (fMax - fMin);
}

void minim(double b, double alf, double alf_min, double C, double&y, double& x1,double& x2) {
	while (alf > alf_min) {
		x2 = x1 + alf * frand(-1, 1);
		if (f(x2, b) < f(x1, b)) {
			x1 = x2;
		}
		alf *= C;
		y = f(x1, b);
	}
}