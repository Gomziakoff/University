#include <iostream>
#include <string>

using namespace std;

class Lab {
private:
	string name_;
	int mark_;
	int& mark = mark_;
	string& name = name_;
	friend void set_object1(Lab &lab,string& name, int& mark);
	friend string getName1(Lab& lab);
	friend int getMark1(Lab& lab);
	friend void print1(Lab& lab);
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
	Lab operator=(const Lab& lab) {
		this->name = lab.name;
		this->mark = lab.mark;
		return *this;
	}
};

void set_object1(Lab& lab, string& name, int& mark) {
	lab.name = name;
	lab.mark = mark;
}
string getName1(Lab& lab) {
	return lab.name;
}
int getMark1(Lab& lab) {
	return lab.mark;
}
void print1(Lab& lab) {
	cout << "Name: " << lab.name << endl;
	cout << "Mark: " << lab.mark << endl;
}


class Students_LAB {
private:
	Lab* lab;
	int size_;
	int& size = size_;
	friend void set_object2(Students_LAB& stud, int& size, Lab* lab);
	friend int& get_size2(Students_LAB& stud);
	friend Lab& get_lab2(Students_LAB& stud, int i);
	friend void print2(Students_LAB& stud);
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
	
	Students_LAB operator=(const Students_LAB& students_lab) {
		this->size = students_lab.size;
		this->lab = new Lab[size];
		for (int i = 0; i < size; i++) {
			this->lab[i] = students_lab.lab[i];
		}
		return *this;
	}
};

void set_object2(Students_LAB& stud,int& size, Lab* lab) {
	stud.size = size;
	stud.lab = new Lab[size];
	for (int i = 0; i < size; i++) {
		stud.lab[i] = lab[i];
	}
}
int& get_size2(Students_LAB& stud) {
	return stud.size;
}
Lab& get_lab2(Students_LAB& stud,int i) {
	return stud.lab[i];
}
void print2(Students_LAB& stud) {
	for (int i = 0; i < stud.size; i++) {
		print1(stud.lab[i]);
	}
}

class Group {
private:
	int count_;
	int& count = count_;
	string* students;
	friend void set_object3(Group& group, int& count, string* students);
	friend int& get_count3(Group& group);
	friend string& get_student3(Group& group, int i);
	friend void print3(Group& group);
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
	Group operator=(const Group& group) {
		this->count = group.count;
		this->students = new string[count];
		for (int i = 0; i < count; i++) {
			this->students[i] = group.students[i];
		}
		return *this;
	}
};

void set_object3(Group& group, int& count, string* students) {
	group.count = count;
	group.students = new string[count];
	for (int i = 0; i < count; i++) {
		group.students[i] = students[i];
	}
}
int& get_count3(Group& group) {
	return group.count;
}
string& get_student3(Group& group, int i) {
	return group.students[i];
}
void print3(Group& group) {
	for (int i = 0; i < group.count; i++) {
		cout << group.students[i] << endl;
	}
}

int main() {
	std::string name = "Lab1";
	int mark = 5;
	Lab lab;
	set_object1(lab, name, mark);
	std::cout << "Name: " << getName1(lab) << std::endl;
	std::cout << "Mark: " << getMark1(lab) << std::endl;
	std::cout << "---------------------------------" << std::endl;
	int count = 3;
	std::string students[3] = { "Ars", "Vlad", "Vova" };
	Group group;
	set_object3(group, count, students);
	print3(group);
	std::cout << "Count: " << get_count3(group) << std::endl;
	std::cout << "Student: " << get_student3(group, 1) << std::endl;
	std::cout << "---------------------------------" << std::endl;
	int size = 2;
	Lab lab1;
	set_object1(lab1, name, mark);
	Lab lab2;
	set_object1(lab2, name, mark);
	Lab lab3;
	set_object1(lab3, name, mark);
	Lab lab4;
	set_object1(lab4, name, mark);
	Lab lab5;
	set_object1(lab5, name, mark);
	Lab lab6;
	set_object1(lab6, name, mark);
}

