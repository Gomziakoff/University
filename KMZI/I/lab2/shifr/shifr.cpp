#include "lib.h"

using namespace std;

int main() {
	while (1) {
		cout << "_-_main menu_-_\n";
		cout << "1.Зашифровать текст\n";
		cout << "2.Расшифровать текст\n";
		cout << "3.Тестовый вариант\n";
		cout << "4.Выход\n";
		int choose;
		cin >> choose;
		switch (choose) {
		case 1: {
			system("cls");
			string filename;
			cout << "Введите название файла для шифрования\n";
			cin >> filename;
			ifstream inputFile(filename);
			string text, key_str;
			getline(inputFile, text);
			getline(inputFile, key_str);
			vector<int> key;
			istringstream iss(key_str);
			int num;
			while (iss >> num) {
				key.push_back(num);
			}
			ofstream outputFile("encrypted_"+filename);
			vector<int> encrypted_text = CBC(text, key, 9);
			for (auto c : CBC(text, key, 9)) {
				outputFile << c << " ";
			}
			outputFile << endl;
			for (auto c : key) {
				outputFile << c << " ";
			}
			cout << "file " << "encrypted_" + filename << " created\n";
			outputFile.close();
			break;
		}
		case 2: {
			system("cls");
			string filename;
			cout << "Введите название файла для расшифрования\n";
			cin >> filename; 
			ifstream inputFile(filename); 
			string text, key_str;
			getline(inputFile, text);
			getline(inputFile, key_str);
			vector<int> ciphertext;
			vector<int> key;
			istringstream iss(key_str);
			istringstream txt(text);
			int num;
			while (iss >> num) {
				key.push_back(num);
			}
			while (txt >> num) {
				ciphertext.push_back(num);
			}
			ofstream outputFile("decrypted_"+filename);
			cout << "file "<<"decrypted_"+filename <<" created\n";
			outputFile << unCBC(ciphertext, key, 9);
			outputFile << endl;
			for (auto c : key) {
				outputFile << c << " ";
			}
			break;
		}
		case 3: {
			system("cls");
			ifstream inputFile("test_text.txt");
			string text,key_str;
			getline(inputFile, text);
			getline(inputFile, key_str);
			vector<int> key;
			istringstream iss(key_str);
			int num;
			while (iss >> num) {
				key.push_back(num);
			}
			ofstream outputFile("encrypted_test_text.txt\n");
			vector<int> encrypted_text = CBC(text, key, 9);
			for (auto c : CBC(text, key, 9)) {
				outputFile << c << " ";
			}
			outputFile << endl;
			for (auto c : key) {
				outputFile << c << " ";
			}
			cout << "file encrypted_test_text.txt created\n";
			outputFile.close();
			outputFile.open("decrypted_test_text.txt");
			outputFile << unCBC(encrypted_text, key, 9);
			outputFile << endl;
			for (auto c : key) {
				outputFile << c << " ";
			}
			cout << "file decrypted_test_text.txt created\n";
			break;
		}
		case 4:
			system("cls");
			return 0;
			break;
		default:
			system("cls");
			cout << "Такого варианта нет\n";
			break;
		}
	}
	
}