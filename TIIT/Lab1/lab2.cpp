#include <iostream>
#include <cmath>
#include <math.h>

#define pi 3.14159265  
#define rad 57.2957795130 
using namespace std;

double func(double x) {

    return x*sin(x);
}

int main()
{
    setlocale(LC_ALL, "Russian");
    double a = 0, b = pi;//левая и правая границы
    double n;//частота разбиений
    double h;//шаг интегрирования
    double integral;
    cout << "Введите частоту n=";
    cin >> n;    
    h = (b - a) / n;
    integral = h*(func(a) + func(b)) / 2;
    for (int i = 1; i <= n; i++) {
        integral = integral + h * func(a+h*i);
    }
    cout << integral;
    return 0;
}