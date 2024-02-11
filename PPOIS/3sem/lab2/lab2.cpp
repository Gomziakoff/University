#include <iostream>
#include <string>

using namespace std;

class Calc {
public: 
	int calc(int a, int b, char op) {
		if (op == '+')
			return a + b;
		else if (op == '-')
			return a - b;
		else if (op == '*')
			return a * b;
		else if (op == '/')
			return a / b;
		else
			return 0;
	}
	int calc(int a, char op) {
		if (op == '!') {
			return factorial(a);
		}
		else return a;
	}

private:
	int factorial(int a) {
		if (a == 0)
			return 1;
		else
			return a * factorial(a - 1);
	}
};

class Navigate {
public:
	void move() { cout << "идти вперед" << endl; }
	void move(int a) { cout << "идти вперед " << a << " шашов налево " << endl; }
	void move(int a, int b) { cout << "идти вперед " << a << " шашов налево " << b << " метров направо" << endl; }
};

class Multiplication {
public:
	int add(int a, int b) { return a * b; }
	int add(int a, int b, float c) { return a * b * (int)c; }
	int add(int a, int b, int c) { return a * b * c; }
};

int main() {
	Calc calc;
	cout << calc.calc(1, 2, '+') << endl;
	cout << calc.calc(1, 2, '-') << endl;
	cout << calc.calc(1, 2, '*') << endl;
	cout << calc.calc(1, 2, '/') << endl;
	cout << calc.calc(3,'!') << endl;
	Navigate go;
	go.move();
	go.move(5);
	go.move(8, 5);
	Multiplication mult;
	cout << mult.add(5, 2) << endl;
	cout << mult.add(5, 2, 2.5f) << endl;
	cout << mult.add(5, 2, 2) << endl;
}