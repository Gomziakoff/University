#include <iostream>
#include <cmath>


using namespace std;

double f(double x, double y,double z) {
	return y * y + y * z * sin(x) + (x * y + 2 * y) * (y * y);
}

double proizvx(double x, double y, double z, double eps=0.0001) {
	return (f(x + eps, y,z) - f(x, y,z)) / eps;
}

double proizvy(double x, double y, double z, double eps = 0.0001) {
	return (f(x, y + eps,z) - f(x, y,z)) / eps;
}

double proizvz(double x, double y, double z, double eps = 0.0001) {
	return (f(x, y, z+eps) - f(x, y, z)) / eps;
}

double grad(double x, double y, double z) {
	return sqrt(pow(proizvx(x, y, z), 2) + pow(proizvy(x, y, z), 2) + pow(proizvz(x, y, z), 2));
}

int main()
{
	srand(time(NULL));
	double x, y, z;
	double x1, y1, z1;
	double xmin, xmax, ymin, ymax, zmin, zmax;
	cout << "введите xmin,xmax:\n";
	cin >> xmin >> xmax;
	cout << "введите ymin,ymax:\n";
	cin >> ymin >> ymax;
	cout << "введите zmin,zmax:\n";
	cin >> zmin >> zmax;
	double e = 0.001;
	int choise;
	x = xmin + rand() % (int)(xmax - xmin + 1);
	y = ymin + rand() % (int)(ymax - ymin + 1);
	z = zmin + rand() % (int)(zmax - zmin + 1);
	cout << "минимум(1) или максимум(2)?";
	cin >> choise;
	switch (choise) {
	case 1:
		while (grad(x,y,z) > e) {
				if (x<xmin || x>xmax || y<ymin || y>ymax || z<zmin || z>zmax) break;
				x1=x - 0.2 * proizvx(x, y, z);
				y1=y - 0.2 * proizvy(x, y, z);
				z1=z - 0.2 * proizvz(x, y, z);
				if (x1<xmin || x1>xmax || y1<ymin || y1>ymax || z1<zmin || z1>zmax) break;
				if (grad(x1, y1, z1) < e)break;
				x = x1;
				y = y1;
				z = z1;
			}
		cout << "min: x = " << x << ", y = " << y << ", z = " << z << ", func = " << f(x, y, z)<<endl;
		break;
	case 2:
		while (grad(x, y, z) > e) {
			if (x<xmin || x>xmax || y<ymin || y>ymax || z<zmin || z>zmax) break;
			x1 = x + 0.2 * proizvx(x, y, z);
			y1 = y + 0.2 * proizvy(x, y, z);
			z1 = z + 0.2 * proizvz(x, y, z);
			if (x1<xmin || x1>xmax || y1<ymin || y1>ymax || z1<zmin || z1>zmax) break;
			if (grad(x, y, z) < e)break;
			x = x1;
			y = y1;
			z = z1;
		}
		cout << "max: x = " << x << ", y = " << y << ", z = " << z << ", func = " << f(x, y, z) << endl;
		break;
	}
}
