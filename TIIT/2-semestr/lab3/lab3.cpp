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



int main() {
	srand(time(NULL));
	double x1,x2,y, b, alf = 100, iter = 1;
	cout << "введите начальную точку\n";
	cin >> x1;
	cout << "введите параметр b [2,10]\n";
	cin >> b;
	cout << iter << "# " << "y = " << f(x1, b) << " x = " << x1 << " alpha=" << alf<<endl;
	while (alf > 0.000001) {
		iter++;
		x2 = x1 + alf * frand(-1, 1);
		if (f(x2, b) < f(x1, b)) {
			x1 = x2;
			cout << iter << "# " << "y = " << f(x1, b) << " x = " << x1 << " alpha=" << alf << endl;
		}
		alf *= 0.8;
	}
}