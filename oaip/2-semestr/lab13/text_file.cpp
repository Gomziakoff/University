#include "text_file.h"

void writetxt(book* array, int N) {
	ofstream f1;
	ifstream index_file;
	index_file.open("Indexes.txt");
	f1.open("file.txt");
	f1 << setw(10) << "№";
	f1 << setw(25) << "name:";
	f1 << setw(25) << "author:";
	f1 << setw(25) << "publisher:";
	f1 << setw(25) << "year:";
	f1 << setw(25) << "pages:";
	f1 << setw(25) << "made in";
	f1 << setw(25) << "are in stock" << endl;
	for (int i = 0; i < N; i++) {
		int k;
		if (index_file.is_open() && !index_file.eof()) {
			index_file >> k;
			index_file >> k;
		}
		else k = i;

		f1 << setw(10) << array[k].reg;
		f1 << setw(25) << array[k].name;
		f1 << setw(25) << array[k].author;
		f1 << setw(25) << array[k].izdat;
		f1 << setw(25) << array[k].year;
		f1 << setw(25) << array[k].pages;
		switch (array[k].proizv) {
		case Belarus:
			f1 << setw(25) << "Беларусь";
			break;
		case Russia:
			f1 << setw(25) << "Россия";
			break;
		case Lituania:
			f1 << setw(25) << "Литва";
			break;
		case Ukraine:
			f1 << setw(25) << "Украина";
			break;
		}
		f1 << setw(25) << array[k].nal.access;
		f1 << endl;
	}
	f1.close();
}
void readtxt(book*& array, int& N) {
	N = 0;
	ifstream f1;
	string s;
	char temp[1024];
	f1.open("file.txt");
	if (!f1.is_open()) {
		input(array, N);
		writetxt(array, N);
	}
	else {
		f1.getline(temp, 1024, '\n');
		for (int i = 0; !f1.eof(); i++) {
			N++;
			book* buf = new book[N - 1];
			for (int j = 0; j < N - 1; j++) {
				buf[j] = array[j];
			}
			array = new book[N];
			for (int j = 0; j < N - 1; j++) {
				array[j] = buf[j];
			}
			f1 >> array[i].reg >> array[i].name >> array[i].author >> array[i].izdat >> array[i].year >> array[i].pages;
			char buf1[20];
			f1 >> buf1;
			if (strcmp(buf1, "Беларусь") == NULL) {
				array[i].proizv = Belarus;
			}
			else if (strcmp(buf1, "Россия") == NULL) {
				array[i].proizv = Russia;
			}
			else if (strcmp(buf1, "Литва") == NULL) {
				array[i].proizv = Lituania;
			}
			else if (strcmp(buf1, "Украина") == NULL) {
				array[i].proizv = Ukraine;
			}
			f1 >> array[i].nal.access;
		}
		N--;
		f1.close();
	}
}
void index_file(int N, int* index) {
	ofstream fout("Indexes.txt");
	for (int i = 0; i < N; i++) {
		fout << i << " " << index[i];
		if (i < N - 1) {
			fout << endl;
		}
	}
	fout.close();
}
void writebin(book* array, int N) {
	ofstream f1;
	f1.open("file.bin", ios_base::binary);
	for (int i = 0; i < N; i++) {
		f1.write((char*)&array[i], sizeof(array[i]));
	}
	f1.close();
}
void readbin(book*& array, int& N) {
	ifstream f1;
	f1.open("file.bin", ios_base::binary);
	N = 0;
	if (!f1.is_open()) {
		input(array, N);
		writebin(array, N);
	}
	else {
		for (int i = 0; !f1.eof() && (f1.peek() != std::ifstream::traits_type::eof()); i++) {
			N++;
			book* tempArr = new book[N - 1];
			for (int j = 0; j < N - 1; j++) {
				tempArr[j] = array[j];
			}
			array = new book[N];
			for (int j = 0; j < N - 1; j++) {
				array[j] = tempArr[j];
			}
			f1.read((char*)&array[i], sizeof(array[i]));
		}
		cout << "Данные считаны из файла.\n";
		f1.close();
	}
}