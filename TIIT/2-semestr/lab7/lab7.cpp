#include <iostream>
#include <fstream>
#include <cmath>
#include <vector>

using namespace std;

double frand(double fMin, double fMax)
{
	double f = (double)rand() / RAND_MAX;
	return fMin + f * (fMax - fMin);
}

double MSE(double y, double y_exp) {
	return sqrt(pow(y - y_exp, 2));
}

double y_exp(double x1, double x2, double x3, double w0, double w1, double w2, double w3) {
	return w0 + w1 * x1 + w2 * x2 + w3 * x3;
}

int main() {
	srand(time(NULL));
	double alf = 100, c = 0.99;
	vector<double> x1,x2,x3;
	vector<double> y;
	double buf;
	ifstream f1,f2;
	f1.open("x.csv");
	f2.open("y.csv");

	while (!f1.eof()) {//считывание данных из файла
		f1 >> buf;
		x1.push_back(buf);
		f1 >> buf;
		x2.push_back(buf);
		f1 >> buf;
		x3.push_back(buf);
	}
	while (!f2.eof()) {
		f2 >> buf;
		y.push_back(buf);
	}
	double w0, w1, w2, w3, w00, w11, w22, w33;//инициализация параметров и параметров для сравнения
	double MSE1 = 100, MSE2;
	w0 = frand(0, 100);
	w1 = frand(0, 100);
	w2 = frand(0, 100);
	w3 = frand(0, 100);
	
	while (alf > 0.0000001) {
		w00 = w0 + alf * frand(-1, 1);
		w11 = w1 + alf * frand(-1, 1);
		w22 = w2 + alf * frand(-1, 1);
		w33 = w3 + alf * frand(-1, 1);
		double sum = 0;
		for (int j = 0; j < y.size(); j++) {
			sum += MSE(y[j], y_exp(x1[j], x2[j], x3[j], w0, w1, w2, w3));
		}
		MSE1 = 1.0 / y.size() * sum;
		sum = 0;
		for (int j = 0; j < y.size(); j++) {
			sum += MSE(y[j], y_exp(x1[j], x2[j], x3[j], w00, w11, w22, w33));
		}
		MSE2 = 1.0 / y.size() * sum;

		if (MSE2 < MSE1) {
			w0 = w00;
			w1 = w11;
			w2 = w22;
			w3 = w33;
			cout << MSE1<< endl;
		}

		alf *= c;
	}
	cout << w0 << " " << w1 << " " << w2 << " " << w3;
	
}