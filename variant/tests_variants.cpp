#include <algorithm>
#include <catch2/catch_approx.hpp>
#include <catch2/catch_test_macros.hpp>
#include <iostream>
#include <numeric>
#include <string>
#include <variant>
#include <vector>

using namespace std;

class Rectangle
{
    int width;
    int height;
public:
    Rectangle(int width, int height) : width(width), height(height) {}

    void draw() const
    {
        cout << "Drawing a rectangle with width " << width << " and height " << height << endl;
    }

    double area() const
    {
        return width * height;
    }
};

class Circle
{
    int radius;
public:
    Circle(int radius) : radius(radius) {}

    void draw() const
    {
        cout << "Drawing a circle with radius " << radius << endl;
    }

    double area() const
    {
        return 3.14159 * radius * radius;
    }
};

class Square
{
    Rectangle rectangle;
public:
    Square(int size) : rectangle(size, size) {}

    void draw() const
    {
        cout << "Drawing a square with size " << rectangle.area() << endl;
    }

    double area() const
    {
        return rectangle.area();
    }
};

TEST_CASE("variant")
{
}