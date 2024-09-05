#include <iostream>
#include <string>

using namespace std;

class ComplexNum {
private:
	float a;
	float b;
public:
	ComplexNum() {
		this->a = 1;
		this->b = 0;
	}
	ComplexNum(float a, float b) {
		this->a = a;
		this->b = b;
	}
	ComplexNum(const ComplexNum& num) {
		this->a = num.a;
		this->b = num.b;
	}
	void setA(float a) { this->a = a; }
	void setB(float b) { this->b = b; }
	float getA() { return a; }
	float getB() { return b; }

	string getComplexNumAsString() {
		string str;
		str = to_string(this->a);
		str += " + ";
		str += to_string(this->b);
		str += "i";
		return str;
	}

	void operator=(const ComplexNum &num) {
		this->a = num.a;
		this->b = num.b;
	}

	ComplexNum operator+(const ComplexNum& num) {
		ComplexNum ans;
		ans.a = this->a + num.a;
		ans.b = this->b + num.b;
		return ans;
	}
	
	ComplexNum operator-(const ComplexNum& num) {
		ComplexNum ans;
		ans.a = this->a - num.a;
		ans.b = this->b - num.b;
		return ans;
	}

	ComplexNum operator*(const ComplexNum& num) {
		ComplexNum ans;
		ans.a = this->a * num.a - this->b * num.b;
		ans.b = this->b * num.b - this->a * num.a;
		return ans;
	}

	ComplexNum operator/(const ComplexNum& num) {
		ComplexNum ans;
		ans.a = (this->a * num.a + this->b * num.b) / \
			(num.a * num.a + num.b * num.b);
		ans.b = (this->b * num.a - this->a * num.b) / \
			(num.a * num.a + num.b * num.b);
		return ans;
	}

	bool operator==(ComplexNum& num) {
		return this->a == num.a && this->b == num.b;
	}
};

int main() {
	ComplexNum a;
	 cout << a.getA() << " + " << a.getB() << "i" << "\n";

	ComplexNum b(3, 5);
	 cout << b.getA() << " + " << b.getB() << "i" << "\n";

	 cout << "Sum:" << (a + b).getComplexNumAsString() << "\n";
	 cout << "Difference:" << (a - b).getComplexNumAsString() << "\n";
	 cout << "Product:" << (a * b).getComplexNumAsString() << "\n";
	 cout << "Quotient:" << (a / b).getComplexNumAsString() << "\n";
}