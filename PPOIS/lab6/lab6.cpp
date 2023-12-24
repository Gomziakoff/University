#include <iostream>
#include <string>

using namespace std;

namespace device1 {
	class Device {
	private:
		int id;
		int type;
	public:
		Device() { id = 0; type = 0; }
		Device(int id, int type) {
			this->id = id;
			this->type = type;
		}
		Device(const Device& dev) {
			this->id = dev.id;
			this->type = dev.type;
		}
		~Device() {}
		void setId(int id) { this->id = id; }
		void setType(int type) { this->type = type; }
		int getId() { return id; }
		int getType() { return type; }
		void print() { cout << " Device1 id:" << getId() << " Device1 type:" << getType() << endl; }
	};
}

namespace device2 {
	class Device {
	private:
		int id;
		int type;
	public:
		Device() { id = 0; type = 0; }
		Device(int id, int type) {
			this->id = id;
			this->type = type;
		}
		Device(const Device& dev) {
			this->id = dev.id;
			this->type = dev.type;
		}
		~Device() {}
		void setId(int id) { this->id = id; }
		void setType(int type) { this->type = type; }
		int getId() { return id; }
		int getType() { return type; }
		void print() { cout << " Device2 id:" << getId() << " Device2 type:" << getType() << endl; }
	};
}
	
namespace hub {
	class Hub : public device1::Device, public device2::Device {
	public:
		Hub() {
			device1::Device::setId(1234);
			device2::Device::setId(1111);
			device1::Device::setType(12);
			device2::Device::setType(21);
		}
		Hub(int id, int type) : device1::Device(id, type), device2::Device(id, type) {}
		Hub(const Hub& hub) : device1::Device(hub), device2::Device(hub) {}
		void setId(int id) {
			device1::Device::setId(1234);
			device2::Device::setId(1111);
		}
		void setType(int type) {
			device1::Device::setType(12);
			device2::Device::setType(21);
		}
		void print() {
			cout << " Device1 id:" << device1::Device::getId() << " Device1 type:" << device1::Device::getType() << endl;
			cout << " Device1 id:" << device2::Device::getId() << " Device1 type:" << device2::Device::getType() << endl;
		}
	};
}

int main() {
	int arr[3] = { 1,2,3 };
	try {
		for (int i = 0; i < 4; i++) {
			std::cout << arr[i] << std::endl;
		}
	}
	catch (std::bad_alloc e) {
		std::cerr << e.what() << '\n';
	}

	device1::Device device1(1, 2);
	device2::Device device2(3, 4);
	hub::Hub hub(6, 7);
	device1.print();
	device2.print();
	hub.print();
}