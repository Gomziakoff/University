#include "menu.h"
void menu() {
	/*while (key != 1 && key != 2) {
		cout << "выберите тип файла (1.bin 2.txt)";
		cin >> key;
	}*/
	int choise = 0, N = 0;
	book* array = new book[N];
	do {
		cout << "Выберите действие:\n";
		cout << "1) Ввод массива структур\n";
		cout << "2) Вывод массива структур\n";
		cout << "3) Сортировка книг \n";
		cout << "4) Вывести список книг с фамилиями авторов, изданных после заданного года.\n";
		cout << "5) Удаление заданной структуры\n";
		cout << "6) Добавление записи\n";
		cout << "7) Выход\n";
		cin >> choise;
		system("cls");

		switch (choise) {
		case 1:
			if (key == 1) readbin(array, N);
			else readtxt(array, N);
			break;
		case 2:
			output(array, N);
			break;
		case 3:
			sort(array, N, key);
			output(array, N);
			cin.ignore();
			break;
		case 4:
			search(array, N);
			break;
		case 5:
			deleted(array, N, key);
			break;
		case 6:
			input(array, N);
			if (key == 1) writebin(array, N);
			else writetxt(array, N);
			break;
		case 7:
			return;
			break;
		}
	} while (true);
}