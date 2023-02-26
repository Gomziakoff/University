#include <iostream>
#include <cmath>
using namespace std;

bool* ob(int* um,int u, bool* maska, bool* maskb) {
	bool* maskc = new bool[u]{0};
	for (int i = 0; i < u; i++) {
		if (maska[i] || maskb[i]) {
			maskc[i] = 1;
		}
	}
	return maskc;
}

bool* peres(int* um, int u, bool* maska, bool* maskb) {
	bool* maskc = new bool[u] {0};
	for (int i = 0; i < u; i++) {
		if (maska[i] && maskb[i]) {
			maskc[i] = 1;
		}
	}
	return maskc;
}

bool* razn(int* um, int u, bool* maska, bool* maskb) {
	bool* maskc = new bool[u] {0};
	for (int i = 0; i < u; i++) {
		if (maska[i] && !maskb[i]) {
			maskc[i] = 1;
		}
	}
	return maskc;
}

bool* sim_razn(int* um, int u, bool* maska, bool* maskb) {
	bool* maskc = new bool[u] {0};
	for (int i = 0; i < u; i++) {
		if ((maska[i] ^ maskb[i])) {
			maskc[i] = 1;
		}
	}
	return maskc;
}

bool* dopoln(int* um, int u, bool* maska) {
	bool* maskc = new bool[u] {0};
	for (int i = 0; i < u; i++) {
		if (!maska[i]) {
			maskc[i] = 1;
		}
	}
	return maskc;
}

void boolean(int *am, int a)
{
	cout << "{ ";
	for (int i = 0; i < pow(2,a); i++) {
		cout << "{";
		for (int j = 0; j < a; j++) {
			if (i & (1 << j))
				cout << am[j] << " ";
		}
		cout << "} ";
	}
	cout << "} \n";
}

void slianie(int *am, int a, int* bm, int b)
{
	int* rez = new int[a + b];
	int i = 0, j = 0, k = 0;
	while (i != a && j != b) {
		if (am[i] < bm[j]) {
			rez[k] = am[i];
			i++;
			k++;
		}
		else
			if (bm[j] < am[i]) {
				rez[k] = bm[j];
				j++;
				k++;
			}
			else
				if (am[i] == bm[j]) {
					rez[k] = am[i];
					k++;
					i++;
					rez[k] = bm[j];
					k++;
					j++;
				}
	}
	if (i == a && j == b);
	else {
		if (i != a)
		{
			while (i != a)
			{
				rez[k] = am[i];
				k++;
				i++;
			}
		}
		if (j != b)
		{
			while (j != b)
			{
				rez[k] = bm[j];
				k++;
				j++;
			}
		}
	}
	for (int i = 0; i < a + b; i++) {
		cout << rez[i] << " ";
	}
	cout << "\n";
}

int main() {
	int u, a, b, variant, end = 0, c;
	bool* pointer;
		cout << "Введите размер универсума :\n ";
		cin >> u;
		int* um = new int[u];
		for (int i = 0; i < u; i++) {
			um[i] = i + 1;
		}
		cout << "Введите размер множества А :\n ";
		cin >> a;
		int* am = new int[a];
		cout << "Введите " << a << " элементов множества А\n";
		for (int i = 0; i < a; i++) {
			cin >> am[i];
		}
		cout << "Введите размер множества B :\n";
		cin >> b;
		int* bm = new int[b];
		cout << "Введите " << b << " элементов множества B\n";
		for (int i = 0; i < b; i++) {
			cin >> bm[i];
		}
		bool* maska = new bool[u] {0};
		bool* maskb = new bool[u] {0};
		for (int i = 0; i < u; i++) {
			for (int j = 0; j < u; j++) {
				if (um[i] == am[j]) maska[i] = 1;
				if (um[i] == bm[j]) maskb[i] = 1;
			}
		}
		while (!end) {
			cout << "\n1. Изменить универсум и множества\n";
			cout << "2. Произвести обьединение А и В \n";
			cout << "3. Произвести пересечение А и В \n";
			cout << "4. Произвести разность А и В\n";
			cout << "5. Произвести симметрическую разность А и В\n";
			cout << "6. Найти дополнение А\n";
			cout << "7. Найти булеан множества А\n";
			cout << "8. Произвести слияние массивов А и В\n";
			cout << "9. Вычислить выражение 8 варианта\n";
			cout << "10. Выход из программы\n";
			cout << "Выберите вариант\n";
			cin >> variant;
			system("cls");
			switch (variant) {
			case 1: {
				cout << "Введите размер универсума :\n ";
				cin >> u;
				int* um = new int[u];
				for (int i = 0; i < u; i++) {
					um[i] = i + 1;
				}
				cout << "Введите размер множества А :\n ";
				cin >> a;
				int* am = new int[a];
				cout << "Введите " << a << " элементов множества А\n";
				for (int i = 0; i < a; i++) {
					cin >> am[i];
				}
				cout << "Введите размер множества B :\n";
				cin >> b;
				int* bm = new int[b];
				cout << "Введите " << b << " элементов множества B\n";
				for (int i = 0; i < b; i++) {
					cin >> bm[i];
				}
				bool* maska = new bool[u] {0};
				bool* maskb = new bool[u] {0};
				for (int i = 0; i < u; i++) {
					for (int j = 0; j < u; j++) {
						if (um[i] == am[j]) maska[i] = 1;
						if (um[i] == bm[j]) maskb[i] = 1;
					}
				}
				break; }
			case 2:
				pointer = ob(um, u, maska, maskb);
				for (int i = 0; i < u; i++) {
					if (pointer[i]) {
						cout << um[i] << " ";
					}
				}cout << endl;
				break;
			case 3:
				pointer = peres(um, u, maska, maskb);
				for (int i = 0; i < u; i++) {
					if (pointer[i]) {
						cout << um[i] << " ";
					}
				}cout << endl;
				break;
			case 4:
				pointer = razn(um, u, maska, maskb);
				for (int i = 0; i < u; i++) {
					if (pointer[i]) {
						cout << um[i] << " ";
					}
				} cout << endl;
				break;
			case 5:
				pointer = sim_razn(um, u, maska, maskb);
				for (int i = 0; i < u; i++) {
					if (pointer[i]) {
						cout << um[i] << " ";
					}
				} cout << endl;
				break;
			case 6:
				pointer = dopoln(um, u, maska);
				for (int i = 0; i < u; i++) {
					if (pointer[i]) {
						cout << um[i] << " ";
					}
				} cout << endl;
				break;
			case 7:
				boolean(am, a);
				break;
			case 8:
				slianie(am, a, bm, b);
				break;
			case 9: {
				cout << "Введите размер множества C :\n ";
				cin >> c;
				int* cm = new int[c];
				cout << "Введите " << c << " элементов множества C: \n";
				for (int i = 0; i < c; i++) {
					cin >> cm[i];
				}
				bool* maskc = new bool[u] {0};
				for (int i = 0; i < u; i++) {
					for (int j = 0; j < u; j++) {
						if (um[i] == cm[j]) maskc[i] = 1;
					}
				}
				pointer = peres(um, u, ob(um, u, maska, maskb), dopoln(um, u, maskc));
				for (int i = 0; i < u; i++) {
					if (pointer[i]) {
						cout << um[i] << " ";
					}
				} cout << endl;
				break; }
			case 10: {
				end = 1;
			}
			}
			
		}
}
