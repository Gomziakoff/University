#include <iostream>
#include <string>

using namespace std;

class Device1 {
private:
	int id;
	int type;
public:
	Device1() { id = 0; type = 0; }
	Device1(int id, int type) {
		this->id = id;
		this->type = type;
	}
	Device1(const Device1& dev) {
		this->id = dev.id;
		this->type = dev.type;
	}
	~Device1(){}
	void setId(int id) { this->id = id; }
	void setType(int type) { this->type = type; }
	int getId(){ return id; }
	int getType() { return type; }
	void print(){ cout << " Device1 id:" << getId() << " Device1 type:" << getType() << endl; }
};

class Device2 {
private:
	int id;
	int type;
public:
	Device2() { id = 0; type = 0; }
	Device2(int id, int type) {
		this->id = id;
		this->type = type;
	}
	Device2(const Device2& dev) {
		this->id = dev.id;
		this->type = dev.type;
	}
	~Device2() {}
	void setId(int id) { this->id = id; }
	void setType(int type) { this->type = type; }
	int getId() { return id; }
	int getType() { return type; }
	void print() { cout << " Device2 id:" << getId() << " Device2 type:" << getType() << endl; }
};

class Hub : public Device1,public Device2{
public:
	Hub() {
		Device1::setId(1234);
		Device2::setId(1111);
		Device1::setType(12);
		Device2::setType(21);
	}
	Hub(int id, int type) : Device1(id,type),Device2(id,type){}
	Hub(const Hub& hub) : Device1(hub), Device2(hub){}
	void setId(int id) { 
		Device1::setId(1234);
		Device2::setId(1111);
	}
	void setType(int type) {
		Device1::setType(12);
		Device2::setType(21);
	}
	void print() {
		cout << " Device1 id:" << Device1::getId() << " Device1 type:" << Device1::getType() << endl;
		cout << " Device1 id:" << Device2::getId() << " Device1 type:" << Device2::getType() << endl;
	}
};

int main() {
	Device1 device1(1, 2);
	Device2 device2(3, 4);
	Hub hub(6, 7);
	device1.print();
	device2.print();
	hub.print();
}