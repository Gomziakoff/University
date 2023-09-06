#include <iostream>
#include <string>
#include <cmath>

using namespace std;

int alltodec(int sys, int length, char *num) {
	int k = length;
	int dec = 0;
	for (int i = 0; i < k; i++) {
		if (num[i] == 'A') num[i] = 10 + '0';
		else if (num[i] == 'B') num[i] = 11 + '0';
		else if (num[i] == 'C') num[i] = 12 + '0';
		else if (num[i] == 'D') num[i] = 13 + '0';
		else if (num[i] == 'E') num[i] = 14 + '0';
		else if (num[i] == 'F') num[i] = 15 + '0';
		if (num[i] - '0' >= sys) {
			cout << "Неправильно введено число\n";
			return 0;
		}
		dec = dec + (num[i] - '0') * (int)pow(sys, (length - i - 1));
	}
	return dec;
}

void dectoall(int num, int to) {
	int length = 0;
	char num1[100], num2[100];
	if (num == to) cout << "10\n"; else
		while (num != 0) {
			num1[length] = (num % to) + '0';
			length++;
			num /= to;
		}
		for (int i = 0; i < length; i++) {
			num2[i] = num1[length - 1 - i];
			if (num2[i] - '0' == 10) num2[i] = 'A';
			else if (num2[i] - '0' == 11) num2[i] = 'B';
			else if (num2[i] - '0' == 12) num2[i] = 'C';
			else if (num2[i] - '0' == 13) num2[i] = 'D';
			else if (num2[i] - '0' == 14) num2[i] = 'E';
			else if (num2[i] - '0' == 15) num2[i] = 'F';
			cout << num2[i];
		}
		cout << endl;	
}

int main()
{
	string check = "Y";
	int sys, to, length;
	char num[100];
	while (check == "Y" || check == "y") {
		cout << "Введите систему счисления вашего числа (до 16) \n";
		cin >> sys;
		cout << "Введите число\n";
		cin >> num;
		cout << "Введите в какую систему перевести число (до 16) \n";
		cin >> to;
		length = strlen(num);
		if (sys > 0 && to > 0 && sys <= 16 && to <= 16)
			dectoall(alltodec(sys, length, num), to);
		else cout << "Неверная система счисления\n";
		cout << "Желаете продолжить? Y/N\n";
		cin >> check;
	}
}
