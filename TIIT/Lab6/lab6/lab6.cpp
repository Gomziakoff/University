#include <iostream>
#include <cmath>

using namespace std;

int main()
{
	 long long int a = 4, p = 11, xi, xj,yi,yj;
	 int zij, zji;
	 cout << "Введите ключ ";
	 cin >> xi;//первое секретное число
	 yi = int(pow(a, xi)) % p;//вычисляем публичное число 1 человека
	 cout << "Введите ключ ";
	 cin >> xj;//второе секретное число
	 zij = int(pow(yi, xj)) % p;//вычисляем секретный ключ 1 человека
	 yj = int(pow(a, xj)) % p;//вычисляем публичное число 2 человека
	 zji = int(pow(yj,xi)) % p;//вычисляем секретный ключ 2 человека
	 cout <<"Секретный ключ 1: " << zij << endl<< " Секретный ключ 2: " << zji;
}

