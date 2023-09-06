#include "interface.h";
void sort(book*& array, int N, int key) //sort by year
{
	cout << "выберите пункт:\n";
	cout << "1 - сортировка по названию книги\n";
	cout << "2 - сортировка по году выпуска\n";
	int choise;
	cin >> choise;
	int* index = new int[N];
	for (int i = 0; i < N; i++) {
		index[i] = i;
	}
	for (int i = 0; i < N - 1; i++) {
		for (int j = i + 1; j < N; j++) {
			if (choise == 2) {
				if (array[index[i]].year > array[index[j]].year)
					swap(index[i], index[j]);
			}
			if (choise == 1) {
				if (strcmp(array[index[j]].name, array[index[i]].name) < 0)
					swap(index[i], index[j]);
			}
		}
	}
	index_file(N, index);
	if (key == 1) writebin(array, N);
	else writetxt(array, N);
	readtxt(array, N);

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
void deleted(book*& array, int& N, int key) {
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
	if (key == 1) writebin(array, N);
	else writetxt(array, N);
	int* tempArray = new int[N];
	for (int i = 0; i < N; i++) {
		tempArray[i] = i;
	}
	index_file(N, tempArray);
	delete[] tempArray;

	writetxt(array, N);
	output(array, N);
}