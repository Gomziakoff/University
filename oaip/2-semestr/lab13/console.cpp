#include "console.h"
void input(book*& array, int& N)
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
	for (int i = N - n; i < N; i++) {
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
		}
		else array[i].nal.access = '-';
		system("cls");
	}
}
void output(book* array, int N)
{
	ifstream index_file;
	index_file.open("Indexes.txt");
	cout << setw(3) << "№";
	cout << setw(20) << "name:";
	cout << setw(20) << "author:";
	cout << setw(20) << "publisher:";
	cout << setw(20) << "year:";
	cout << setw(20) << "pages:";
	cout << setw(20) << "made in";
	cout << setw(20) << "are in stock" << endl;
	for (int i = 0; i < N; i++) {
		int k;
		if (index_file.is_open() && !index_file.eof()) {
			index_file >> k;
			index_file >> k;
		}
		else k = i;
		cout << setw(10) << array[i].reg;
		cout << setw(20) << array[i].name;
		cout << setw(20) << array[i].author;
		cout << setw(20) << array[i].izdat;
		cout << setw(20) << array[i].year;
		cout << setw(20) << array[i].pages;
		switch (array[i].proizv) {
		case Belarus:
			cout << setw(20) << "Беларусь";
			break;
		case Russia:
			cout << setw(20) << "Россия";
			break;
		case Lituania:
			cout << setw(20) << "Литва";
			break;
		case Ukraine:
			cout << setw(20) << "Украина";
			break;
		}
		cout << setw(20) << array[i].nal.access;
		cout << endl;
	}
}