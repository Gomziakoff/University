#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <cstring>

// Базовый класс Транспортное средство (Vehicle)
class Vehicle {
protected:
    int speed;
    char* fuelType;  // Динамическая память
    int capacity;

public:
    Vehicle(int spd, const char* fuel, int cap) : speed(spd), capacity(cap) {
        fuelType = new char[strlen(fuel) + 1];
        strcpy(fuelType, fuel);
    }

    Vehicle(const Vehicle& other) {
        speed = other.speed;
        capacity = other.capacity;
        fuelType = new char[strlen(other.fuelType) + 1];
        strcpy(fuelType, other.fuelType);
    }

    Vehicle& operator=(const Vehicle& other) {
        if (this == &other) return *this;
        delete[] fuelType;
        speed = other.speed;
        capacity = other.capacity;
        fuelType = new char[strlen(other.fuelType) + 1];
        strcpy(fuelType, other.fuelType);
        return *this;
    }

    virtual ~Vehicle() {
        delete[] fuelType;
    }

    virtual void display() const {
        std::cout << "Speed: " << speed << " km/h, Fuel: " << fuelType << ", Capacity: " << capacity << std::endl;
    }
};

// Класс Автомобиль (Car)
class Car : public Vehicle {
private:
    char* brand;

public:
    Car(int spd, const char* fuel, int cap, const char* brandName)
        : Vehicle(spd, fuel, cap) {
        brand = new char[strlen(brandName) + 1];
        strcpy(brand, brandName);
    }

    Car(const Car& other) : Vehicle(other) {
        brand = new char[strlen(other.brand) + 1];
        strcpy(brand, other.brand);
    }

    Car& operator=(const Car& other) {
        if (this == &other) return *this;
        Vehicle::operator=(other);
        delete[] brand;
        brand = new char[strlen(other.brand) + 1];
        strcpy(brand, other.brand);
        return *this;
    }

    ~Car() {
        delete[] brand;
    }

    void display() const override {
        Vehicle::display();
        std::cout << "Brand: " << brand << std::endl;
    }
};

// Класс Loadable (интерфейс для грузоподъемности)
class Loadable {
protected:
    int* loadCapacity;  // Динамическая память

public:
    Loadable(int loadCap) {
        loadCapacity = new int(loadCap);
    }

    virtual ~Loadable() {
        delete loadCapacity;
    }

    virtual void load() const {
        std::cout << "Load capacity: " << *loadCapacity << " tons" << std::endl;
    }
};

// Класс Грузовик (Truck), множественное наследование
class Truck : public Vehicle, public Loadable {
private:
    char* model;

public:
    Truck(int spd, const char* fuel, int cap, int loadCap, const char* modelName)
        : Vehicle(spd, fuel, cap), Loadable(loadCap) {
        model = new char[strlen(modelName) + 1];
        strcpy(model, modelName);
    }

    Truck(const Truck& other) : Vehicle(other), Loadable(other) {
        model = new char[strlen(other.model) + 1];
        strcpy(model, other.model);
    }

    Truck& operator=(const Truck& other) {
        if (this == &other) return *this;
        Vehicle::operator=(other);
        Loadable::operator=(other);
        delete[] model;
        model = new char[strlen(other.model) + 1];
        strcpy(model, other.model);
        return *this;
    }

    ~Truck() {
        delete[] model;
    }

    void display() const override {
        Vehicle::display();
        Loadable::load();
        std::cout << "Model: " << model << std::endl;
    }
};

// Меню с динамическим полиморфизмом
void showMenu() {
    Vehicle* vehiclePtr = nullptr;
    int choice;
    while (true) {
        std::cout << "\nВыберите тип транспортного средства:\n";
        std::cout << "1. Автомобиль\n";
        std::cout << "2. Грузовик\n\n";
        std::cin >> choice;

        if (choice == 1) {
            vehiclePtr = new Car(180, "Gasoline", 5, "Toyota");
        }
        else if (choice == 2) {
            vehiclePtr = new Truck(120, "Diesel", 3, 10, "MAN");
        }
        else {
            break;
        }

        if (vehiclePtr != nullptr) {
            vehiclePtr->display();  // Полиморфизм
            delete vehiclePtr;
        }
    }
}

int main() {
    showMenu();
    return 0;
}
