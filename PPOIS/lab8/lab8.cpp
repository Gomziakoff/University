#include <iostream>
#include <fstream>
#include <string>

using namespace std;

class InOutConsole {
private:
	string str;
public:
	InOutConsole() { str = "Hello world!"; }
	InOutConsole(string str) { this->str = str; }
	InOutConsole(const InOutConsole& inp) { this->str = inp.str; }
	~InOutConsole(){}
	void read() {
		cout << "enter sstring:" << endl;
		getline(cin, str);
	}
	void write() {
		puts(str.c_str());
	}
};

class InOutFile {
private:
	string str;
public:
	InOutFile() { str = "Hello world!"; }
	InOutFile(string str) { this->str = str; }
	InOutFile(const InOutFile& inp) { this->str = inp.str; }
	~InOutFile() {}
	void read() {
		ifstream file("file1.txt");
		getline(file, str);
		cout << str << endl;
		file.close();
	}
	void write() {
		ofstream file("file1.txt");
		file << str;
		file.close();
	}
};

class InOutArrayFile {
private:
	string str[3];
public:
	InOutArrayFile() { str[0] = "Hello world!"; }
	InOutArrayFile(string str[]) {
		for (int i = 0; i < 3; i++)
			this->str[i] = str[i];
	}
	InOutArrayFile(const InOutArrayFile& inp) { 
		for (int i = 0; i < 3; i++)
			this->str[i] = inp.str[i];
	}
	~InOutArrayFile() {}
	void read() {
		ifstream file("file2.txt");
		for (int i = 0; i < 3; i++) {
			getline(file, str[i]);
			cout << str[i] << " ";
		}
		file.close();
	}
	void write() {
		ofstream file("file2.txt");
		for (int i = 0; i < 3; i++) {
			file << str[i] << endl;
		}
		file.close();
	}
};

int main() {
	InOutConsole cons;
	InOutFile file("Information");
	string ar[3] = { "one ","two ","three " };
	InOutArrayFile arfile(ar);
	cons.read();
	cons.write();
	file.write();
	file.read();
	arfile.write();
	arfile.read();
}