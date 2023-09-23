#include <iostream>
#include <string>

using namespace std;

class Lab {
private:
	string name_;
	int mark_;
	int& mark = mark_;
	string& name = name_;
public:
	Lab() {
		name = "lesha";
		mark = 2;
	}
	Lab(string& name, int& mark) {
		this->name = name;
		this->mark = mark;
	}
	Lab(const Lab& lab) {
		this->name = lab.name;
		this->mark = lab.mark;
	}
	~Lab() {
		cout << "destructor" << endl;
	}
public:
	void set_object(string& name, int& mark) {
		this->name = name;
		this->mark = mark;
	}
	string getName() {
		return this->name;
	}
	int getMark() {
		return this->mark;
	}
	void print() {
		cout << "Name: " << name << endl;
		cout << "Mark: " << mark << endl;
	}
	Lab operator=(const Lab& lab) {
		this->name = lab.name;
		this->mark = lab.mark;
		return *this;
	}
};

class Students_LAB {
private:
	Lab* lab;
	int size_;
	int& size = size_;
public:
	Students_LAB() {
		size = 1;
		lab = new Lab[size];
	}
	Students_LAB(int& size, Lab* lab) {
		this->size = size;
		this->lab = new Lab[size];
		for (int i = 0; i < size; i++) {
			this->lab[i] = lab[i];
		}
	}
	Students_LAB(const Students_LAB& students_lab) {
		this->size = students_lab.size;
		this->lab = new Lab[size];
		for (int i = 0; i < size; i++) {
			this->lab[i] = students_lab.lab[i];
		}
	}
	~Students_LAB() {
		delete[] lab;
	}
	void set_object(int& size, Lab* lab) {
		this->size = size;
		this->lab = new Lab[size];
		for (int i = 0; i < size; i++) {
			this->lab[i] = lab[i];
		}
	}
	int& get_size() {
		return size;
	}
	Lab& get_lab(int i) {
		return lab[i];
	}
	void print() {
		for (int i = 0; i < size; i++) {
			lab[i].print();
		}
	}
	Students_LAB operator=(const Students_LAB& students_lab) {
		this->size = students_lab.size;
		this->lab = new Lab[size];
		for (int i = 0; i < size; i++) {
			this->lab[i] = students_lab.lab[i];
		}
		return *this;
	}
};

class Group {
private:
	int count_;
	int& count = count_;
	string* students;
public:
	Group() {
		count = 1;
		students = new string[count];
		students[0] = "Ars";
	}
	Group(int& count, string* students) {
		this->count = count;
		this->students = new string[count];
		for (int i = 0; i < count; i++) {
			this->students[i] = students[i];
		}
	}
	Group(const Group& group) {
		this->count = group.count;
		this->students = new string[count];
		for (int i = 0; i < count; i++) {
			this->students[i] = group.students[i];
		}
	}
	~Group() {
		delete[] students;
	}
	void set_object(int& count, string* students) {
		this->count = count;
		this->students = new string[count];
		for (int i = 0; i < count; i++) {
			this->students[i] = students[i];
		}
	}
	int& get_count() {
		return count;
	}
	string& get_student(int i) {
		return students[i];
	}
	void print() {
		for (int i = 0; i < count; i++) {
			cout << students[i] << endl;
		}
	}
	Group operator=(const Group& group) {
		this->count = group.count;
		this->students = new string[count];
		for (int i = 0; i < count; i++) {
			this->students[i] = group.students[i];
		}
		return *this;
	}
};

int main() {
	string name = "Ivanov";								
	int mark = 5;						
	Lab lab1;						
	lab1.print();						
	cout << endl;				
	Lab lab2(name, mark);				
	lab2.print();						
	cout << endl;				
	Lab lab3(lab2);					
	lab3.print();						
	cout << endl;				
	lab3.set_object(name, mark);			
	lab3.print();						
	cout << endl;				
	lab3 = lab2;						
	lab3.print();
	cout << endl;				
	cout << lab3.getName() << endl;
	cout << lab3.getMark() << endl;
	cout << endl;
	int size = 2;
	Lab *lab = new Lab[size];
	lab[0] = lab1;
	lab[1] = lab2;
	Students_LAB students_lab1;				
	students_lab1.print();					
	cout << endl;
	Students_LAB students_lab2(size, lab);		
	students_lab2.print();					
	cout << endl;				
	Students_LAB students_lab3(students_lab2);
	students_lab3.print();
	cout << endl;
	students_lab3.set_object(size, lab);
	students_lab3.print();
	cout << endl;
	students_lab3 = students_lab2;
	students_lab3.print();
	cout << endl;
	cout << students_lab3.get_size() << endl;
	students_lab3.get_lab(0).print();
	students_lab3.get_lab(1).print();
	cout << endl;
	delete[] lab;
	Group group1;
	group1.print();
	cout << endl;
	int count = 2;
	string students[2] = { "Petrov", "Sidorov" };
	Group group2(count, students);
	group2.print();
	cout << endl;
	Group group3(group2);
	group3.print();
	cout << endl;
	group3.set_object(count, students);
	group3.print();
	cout << endl;
	cout << group3.get_count() << endl;
	cout << group3.get_student(0) << endl;
	cout << endl;
	group3 = group2;
	group3.print();
	cout << endl;
}

