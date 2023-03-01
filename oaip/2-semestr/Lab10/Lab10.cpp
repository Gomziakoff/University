#include <iostream>
#include <string>
#include <windows.h>
#include <iomanip>
#include <sstream>
using namespace std;

enum Proizv {Belarus = 1, Russia, Lituania, Ukraine};

union Nalichie {
	int check;
	char access;
};

struct book {
	int reg;
	string author,
		name,
		izdat;
	int year: 16;
	unsigned pages;
	Nalichie nal;
	Proizv proizv;
};



void input(book*& array, int &N) 
{
	int n;
	cout << "Введите количество элементов массива\n";
	cin >> n;
	N = N + n;
	book* tempArr = new book[N]{};
	for (int i = 0; i < N - n; i++)
		tempArr[i] = array[i];
	array = new book[N];
	for (int i = 0; i < N - n; i++)
		array[i] = tempArr[i];
	
	system("cls");
	for (int i = N-n; i < N; i++) {
		array[i].reg = i + 1;
		cout << "1) название книги\n";
		cin >> array[i].name;
		cout << "2) автор книги\n";
		cin >> array[i].author;
		cout << "3) название издательства\n";
		cin >> array[i].izdat;
		cout << "4) год издания книги\n";
		int year = 0;
		cin >> year;
		array[i].year = year;
		cout << "5) количество страниц\n";
		cin >> array[i].pages;
		int choise;
		cout << "6) страна поставщик (1 - Беларусь, 2 - Россия, 3 - Латвия, 4 - Украина)\n";
		cin >> choise;
		array[i].proizv = (Proizv)choise;
		cout << "7) есть ли книга в наличии? (1 - да, 2 - нет)\n";
		cin >> array[i].nal.check;
		if (array[i].nal.check == 1) {
			array[i].nal.access = '+';
		} else array[i].nal.access = '-';
		system("cls");
	}
}

void output(book* array, int N) 
{
	cout << setw(10) << "№";
	cout << setw(25) << "name:";
	cout << setw(25) << "author:";
	cout <<setw(25) << "publisher:";
	cout << setw(25) << "year:";
	cout << setw(25) << "pages:";
	cout << setw(25) << "made in";
	cout << setw(25) << "are in stock" << endl;
	for (int i = 0; i < N; i++) {
		cout << setw(10) << array[i].reg;
		cout << setw(25) << array[i].name;
		cout << setw(25) << array[i].author;
		cout << setw(25) << array[i].izdat;
		cout << setw(25) << array[i].year;
		cout << setw(25) << array[i].pages;
		switch (array[i].proizv) {
		case Belarus:
			cout << setw(25) << "Беларусь" ;
			break;
		case Russia:
			cout << setw(25) << "Россия" ;
			break;
		case Lituania:
			cout << setw(25) << "Литва" ;
			break;
		case Ukraine:
			cout << setw(25) << "Украина" ;
			break;
		}
		cout << setw(25) << array[i].nal.access;
		cout << endl;
	}
}

void sort(book*& array, int N) //sort by year
{
	for (int i = 0; i < N - 1; i++) {
		for (int j = i + 1; j < N; j++) {
			if (array[i].year > array[j].year) 
				swap(array[i], array[j]);
		}
	}
}

void search(book* array, int N) //search books after X year
{
	int fond;
	cout << "Введите книги с какого года вам показать?\n";
	cin >> fond;
	for (int i = 0; i < N; i++) {
		if (array[i].year >= fond) {
			cout << "№" << array[i].reg << "; ";
			cout << "Название: " << array[i].name << "; ";
			cout << "Автор: " << array[i].author << "; ";
			cout << "Издательство: " << array[i].izdat << "; ";
			cout << "Год выпуска: " << array[i].year << "; ";
			cout << "Кол-во страниц: " << array[i].pages << "; ";
			switch (array[i].proizv) {
			case Belarus:
				cout << "Поставщик: " << "Беларусь" << "; ";
				break;
			case Russia:
				cout << "Поставщик: " << "Россия" << "; ";
				break;
			case Lituania:
				cout << "Поставщик: " << "Литва" << "; ";
				break;
			case Ukraine:
				cout << "Поставщик: " << "Украина" << "; ";
				break;
			}
			cout << "Есть в наличии: " << array[i].nal.access << "; ";
			cout << endl;
		}
	}
}

void deleted(book*& array, int& N) {
	int find;
	output(array, N);
	cout << "Введите номер книги которую нужно удалить: ";
	cin >> find;
	for (int i = 0; i < N; i++) {
		if (array[i].reg == find) {
			N--;
			for (int j = i; j < N; j++)
				array[j] = array[j + 1];
			i--;
		}
	}
}

int menu()
{
	int choise = 0, N = 0;
	book *array = new book[N];
	do {
		cout << "Выберите действие:\n";
		cout << "1) Ввод массива структур\n";
		cout << "2) Вывод массива структур\n";
		cout << "3) Сортировка книг по году выпуска \n";
		cout << "4) Вывести список книг с фамилиями авторов, изданных после заданного года.\n";
		cout << "5) Удаление заданной структуры\n";
		cout << "6) Выход\n";
		cin >> choise;
		system("cls");
		
		switch (choise) {
		case 1:
			input(array, N);
			break;
		case 2:
			output(array, N);
			break;
		case 3:
			sort(array, N);
			system("cls");
			output(array, N);
			break;
		case 4:
			search(array, N);
			break;
		case 5:
			deleted(array, N);
			break;
		case 6:
			return 0;
			break;
		}
	} while (true);

}
int main()
{
	menu();
	return 0;
}
