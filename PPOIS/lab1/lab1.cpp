#include <iostream>
#include <string>

using namespace std;

class Product {
private:
	string name;
	int cost;
	int valid_until[3];
public:
	Product() {
		this->name = "noname";
		this->cost = 0;
		this->valid_until[0] = 1;
		this->valid_until[1] = 1;
		this->valid_until[2] = 1999;
	}
	Product(string name, int cost, int valid_until[]) {
		this->name = name;
		this->cost = cost;
		for (int i = 0; i < 3; i++) {
			this->valid_until[i] = valid_until[i];
		}
	}
	Product(const Product &product) {
		this->name = product.name;
		this->cost = product.cost;
		for (int i = 0; i < 3; i++) {
			this->valid_until[i] = product.valid_until[i];
		}
	}
	void setName(string name) { this->name = name; }
	void setCost(int cost) { this->cost = cost; }
	void setValid(int valid_until[]) {
		for (int i = 0; i < 3; i++) {
			this->valid_until[i] = valid_until[i];
		}
	}
	string getName() { return this->name; }
	int getCost() { return this->cost; }
	int* getValid() { return this->valid_until; }
	void operator=(const Product& product) {
		this->name = product.name;
		this->cost = product.cost;
		for (int i = 0; i < 3; i++) {
			this->valid_until[i] = product.valid_until[i];
		}
	}
	void operator--(int) {
		this->name = "noname";
		this->cost = 0;
		this->valid_until[0] = 1;
		this->valid_until[1] = 1;
		this->valid_until[2] = 1999;
	}
	bool operator==(Product& obj) {
		return this->name == obj.name;
	}
};

int main() {
	Product soup;
	cout << soup.getName() << endl;

	int date[3] = { 10,12,2024 };
	Product pr1("potato", 5, date);
	cout << pr1.getName() << endl;

	cout << (soup == pr1);
	return 0;
}