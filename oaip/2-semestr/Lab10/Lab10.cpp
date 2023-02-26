#include <iostream>
#include <windows.h>
#include <iomanip>

enum Continent { Europe = 1, Africa, North_America, South_America, Australia, Asia };

union Sea {
	int num;
	char access;
};

struct Country {
	char name[20],
		capital[20],
		currency[20];
	int population;
	Continent continent;
	Sea sea;
	unsigned goverment : 2;
};

int menu();                                //организация меню
void input(Country*& array, int& N);       //ввод данных массива структуры
void output(Country* array, int N);        //вывод данных массива структур
void sort(Country* array, int N);          //сортировка во алфавиту по фамилии
int search(Country* array, int N);         //поиск по заданному параметру
void deleted(Country* array, int& N);      //удаление структуры из массива структур

int main() {
	SetConsoleCP(1251);
	SetConsoleOutputCP(1251);
	menu();
	return 0;
}

int menu() {
	int choise, N = 0;
	Country* array = new Country[N];
	do {
		std::cout << "Выберите действие:\n";
		std::cout << "1) Ввод массива структур\n";
		std::cout << "2) Вывод массива структур\n";
		std::cout << "3) Сортировка массива структур по алфавит\n";
		std::cout << "4) Поиск страны с наибольшим населением\n";
		std::cout << "5) Удаление заданной структуры\n";
		std::cout << "6) Выход\n";
		std::cin >> choise;
		system("cls");

		switch (choise)
		{
		case 1:
			input(array, N);
			break;
		case 2:
			std::cin.ignore();
			output(array, N);
			break;
		case 3:
			std::cin.ignore();
			sort(array, N);
			output(array, N);
			break;
		case 4:
			std::cin.ignore();
			for (int i = 0; i < N; i++) {
				if (array[i].population == search(array, N)) {
					std::cout << "Страна с самым большим населением:\n";
					std::cout << array[i].name << " (" << array[i].population << " человек)\n";
				}
			}
			break;
		case 5:
			std::cin.ignore();
			deleted(array, N);
			break;
		case 6: return 0;
		}
		std::cin.ignore();
		system("cls");
	} while (true);
	delete[] array;
}

void input(Country*& array, int& N) {
	int n;
	std::cout << "Введите количество стран: \n";
	std::cin >> n;
	N = N + n;
	Country* tempArr = new Country[N];
	for (int i = 0; i < N - n; i++)
		tempArr[i] = array[i];
	array = new Country[N];
	for (int i = 0; i < N - n; i++)
		array[i] = tempArr[i];
	delete tempArr;
	system("cls");
	std::cin.ignore();
	for (int i = N - n; i < N; i++) {
		int choise;
		std::cout << "1) Название страны: ";
		gets_s(array[i].name);
		std::cout << "2) Столица: ";
		gets_s(array[i].capital);
		std::cout << "3) Валюта: ";
		gets_s(array[i].currency);
		std::cout << "4) Континент (1-Европа, 2-Африка, 3-Северная Америка, 4-Южная Америка, 5-Австралия, 6-Азия): ";
		std::cin >> choise;
		array[i].continent = (Continent)choise;
		std::cout << "5) Выход к морю (1-есть, 2-нет): ";
		std::cin >> array[i].sea.num;
		if (array[i].sea.num == 1)
			array[i].sea.access = '+';
		if (array[i].sea.num == 2)
			array[i].sea.access = '-';
		std::cout << "6) Население: ";
		std::cin >> array[i].population;
		std::cout << "7) Форма правления (1-монархия, 2-республика): ";
		std::cin >> choise;
		array[i].goverment = choise;
		std::cin.ignore();
		system("cls");
	}
}

void output(Country* array, int N) {
	std::cout << std::setw(8) << std::left << "№" << std::setw(15) << std::left << "Страна" << std::setw(15) << std::left << "Столица" << std::setw(15) << std::left << "Валюта";
	std::cout << std::setw(20) << std::left << "Континент" << std::setw(15) << std::left << "Выход к морю" << std::setw(15) << std::left << "Население" << std::setw(15) << std::left << "Форма правления\n";
	for (int i = 0; i < N; i++) {
		std::cout << i + 1 << "\t";
		std::cout << std::setw(15) << std::left << array[i].name;
		std::cout << std::setw(15) << std::left << array[i].capital;
		std::cout << std::setw(15) << std::left << array[i].currency;
		switch (array[i].continent) {
		case Europe:
			std::cout << std::setw(20) << std::left << "Европа";
			break;
		case Africa:
			std::cout << std::setw(20) << std::left << "Африка";
			break;
		case North_America:
			std::cout << std::setw(20) << std::left << "Северная Америка";
			break;
		case South_America:
			std::cout << std::setw(20) << std::left << "Южная Америка";
			break;
		case Australia:
			std::cout << std::setw(20) << std::left << "Австралия";
			break;
		case Asia:
			std::cout << std::setw(20) << std::left << "Азия";
			break;
		}
		std::cout << std::setw(15) << std::left << array[i].sea.access;
		std::cout << std::setw(15) << std::left << array[i].population;
		if (array[i].goverment == 1)
			std::cout << std::setw(15) << std::left << "монархия";
		if (array[i].goverment == 2)
			std::cout << std::setw(15) << std::left << "республика";
		std::cout << std::endl;
	}
}

void sort(Country* array, int N) {
	for (int i = 0; i < N - 1; i++) {
		for (int j = i; j < N; j++) {
			if (strcmp(array[j].name, array[i].name) < 0)
				std::swap(array[i], array[j]);
		}
	}
}

int search(Country* array, int N) {
	int max = array[0].population, max_ind = 0;
	for (int i = 1; i < N; i++) {
		if (array[i].population > max)
			max = array[i].population;
	}
	return max;
}

void deleted(Country* array, int& N) {
	char Name[20];
	std::cout << "Введите название страны, которую хотите удалить.\n";
	gets_s(Name);
	for (int i = 0; i < N; i++) {
		if (strcmp(array[i].name, Name) == 0) {
			N--;
			for (int j = i; j < N; j++)
				array[j] = array[j + 1];
			i--;
		}
	}
	Country* tempArr = new Country[N];
	for (int i = 0; i < N; i++)
		tempArr[i] = array[i];
	array = new Country[N];
	for (int i = 0; i < N; i++)
		array[i] = tempArr[i];
	delete tempArr;
}

