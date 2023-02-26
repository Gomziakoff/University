#include <iostream>
using namespace std;
int main()
{
	unsigned char a[14];
	int i, b[14];
	int j;
	cout << "vvedite odno chislo shtrihcoda, esli chislo propusheno vvedite ?" << endl;
	int sr = 0, s1 = 0, sn = 0, s = 0, t = 0, p = 0 ,proverka = 0, num = 0;
	for (int i = 0; i < 13; i++) {
		cin >> a[i];
	}
	for (int i = 0; i < 13; i++) {
		if (a[i] == '?') {
			a[i] = '0';
			num = i;
			proverka = 1;
		}
	}
	for (int i = 0; i < 13; i++) {
		b[i] =  a[i]-'0';//я не знаю как это работает
		cout << b[i];
	}
	cout << " ваш штрих-код " << endl;
		for (int j = 0; j < 12; j++) {
			if (j % 2 == 1)
				sr = sr + b[j];
		}
		s1 = sr * 3;
		for (int j = 0; j <= 11; j++) {
			if (j % 2 == 0)
				s1 += b[j];
		}
		s = s1 + sn;
		t = s % 10;
		p = 10 - t;
		if (proverka == 0) {
			if (p == b[12]) {
				cout << "штрихкод верный" << endl;
			}
			else
			{

				cout << "штрихкод неверный "<<endl;
				for (int i = 0; i < 13; i++) {
					cout << b[i];
					b[12] = p;
				}
				cout << " вот его правильный вариант";
			}
		}
		if (proverka == 1) {
			while (10 - s % 10 != b[12]) {
				if (num == 12) {
					b[12] = 10 - s % 10;
				}
				else if (num % 2 ) {
						s = s + 3;
						b[num]++;
				}
				else if (num % 2 == 0) {
							s = s + 1;
							b[num]++;
						}
			}
			cout << "вот готовый штрихкод ";
			for (int i = 0; i < 13; i++) {
				cout << b[i];
			}

		}
}//string