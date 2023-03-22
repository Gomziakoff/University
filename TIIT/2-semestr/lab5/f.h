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
	double x, y,z;
	x = frand(0, 1);
	y = frand(0, 1);
	z = cos(2 * 3.14 * x) * sqrt(-2 * log(y));
	return m + sigm * z;
}

double U(double c, double d) {//равномерная генерация
	double min = c - d;
	double max = c + d;
	return frand(min,max);
}

double E(double lambd) {//экспоненциальное разделение
	return (log(1-frand(0,1)) / (-lambd));
}

class virus {
public:
	double I = U(20, 20);
};
virus ebola;
class enviroment {
public:
	double AP = N(25, 5.5);
	double getM() {
		M = N(20, 3.5);
		return M;
	};
	double T = N(3, 1.5);
private:
	double M;
};
enviroment reg;
class agent {
public:
	double getSC() {
		return U(m, 6);
	}
	int getRT() {
		return 1 + U(20 - 1.5 * HA - 0.5 * Mself, 5 - 0.25 * HA - 0.1 * Mself);
	}
	double getDR() {
		return A * 0.05 + U(15 - 1.5 * HA - 0.5 * Mself, 3 - 0.25 * HA - 0.1 * Mself);
	}
	agent() {
		status = 2;
		m = frand(0, 50);
		R = 10 - E(0.5);
		HA = N(6, 0.5);
		A = N(reg.AP, 5.5);
		Mself = reg.getM();
		RT = 1 + U(20 - 1.5 * HA , 5 - 0.25 * HA);
		DR = (A * 0.05 + U(15 - 1.5 * HA, 3 - 0.25 * HA));
	}
	bool started = 0;//перепроверил ли человек время смерти
	int m;
	int Mself;//эффективность лечения этого человека
	int status;
	double SC;
	double R;
	double HA;
	double A;
	int RT;
	double DR;
};
