#pragma once
#pragma once
#include <cmath>
#include <iostream>
#include <random>
#include <vector>
using namespace std;

double frand(double fMin, double fMax)
{
	double f = (double)rand() / RAND_MAX;
	return fMin + f * (fMax - fMin);
}

double N(double m, double sigm) {//Преобразование Бокса — Мюллера, нормальная генерация
	double x, y, z;
	x = frand(0, 1);
	y = frand(0, 1);
	z = cos(2 * 3.14 * x) * sqrt(-2 * log(y));
	if (m + sigm * z > 0) {
		return m + sigm * z;
	}
	else return 0;
}

double U(double c, double d) {//равномерная генерация
	double min = c - d;
	double max = c + d;
	return frand(min, max);
}

