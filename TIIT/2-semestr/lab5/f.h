#pragma once
#include <cmath>

using namespace std;

double frand(double fMin, double fMax)
{
	double f = (double)rand() / RAND_MAX;
	return fMin + f * (fMax - fMin);
}

double N(double m, double sigm) {//Преобразование Бокса — Мюллера, нормальная генерация
	double x, y,z;
	x = frand(0, 1);
	y = frand(0, 1);
	z = cos(2 * 3.14 * x) * sqrt(-2 * log(y));
	return m + sigm * z;
}

double U(double c, double d) {//равномерная генерация
	double min = c - d;
	double max = c + d;
	double f = (double)rand() / RAND_MAX;
	return min + f * (max - min);
}

double E(double lambd) {
	return (log(1-frand(0,1)) / (-lambd));
}

double SC(double m) {
	return U(m, 6);
}

double R()