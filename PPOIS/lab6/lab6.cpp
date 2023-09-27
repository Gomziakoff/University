#include <iostream>
#include <string>

using namespace std;
namespace shape {
    class Shape {
    public:
        Shape() {
            color = "nothing";
        }
        Shape(const string& color) : color(color) {}
        Shape(const Shape& shape) : color(shape.color) {}
        ~Shape() {}

        virtual double area() const = 0;
        virtual double perimetr() const = 0;

        string getColor() {
            return color;
        }
        void setColor(string color) {
            this->color = color;
        }
    private:
        string color;
    };
}

namespace circle {
    class Circle : public shape::Shape {
    public:
        Circle(const string& color, double radius) : Shape(color), radius(radius) {};

        double area() const override {
            return 3.14 * radius * radius;
        }

        double perimetr() const override {
            return 2 * 3.14 * radius;
        }

        double getRadius() { return radius; }
        void setRadius(double radius) { this->radius = radius; }
        void print() {
            cout << "circle:" << endl << " color: " << getColor() << "; radius: " << radius << endl << "area: " << area() << " perimetr: " << perimetr() << endl;
        }
    private:
        double radius;
    };
}

namespace rectangle {
    class Rectangle : public shape::Shape {
    public:
        Rectangle(const string& color, double len, double width) : Shape(color), len(len), width(width) {};

        double area() const override {
            return len * width;
        }
        double perimetr() const override {
            return 2 * (len + width);
        }
        double getLen() { return len; }
        double getWidth() { return width; }
        void setLen(double len) { this->len = len; }
        void setWidth(double width) { this->width = width; }
        void print() {
            cout << "rectangle" << endl << "color: " << getColor() << "; len: " << len << "; width: " << width << endl << "area: " << area() << " perimetr: " << perimetr() << endl;
        }
    private:
        double len;
        double width;
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
    rectangle::Rectangle greenrect("green", 10, 5);
    circle::Circle redcircle("red", 4);
    greenrect.print();
    redcircle.print();
    return 0;
}