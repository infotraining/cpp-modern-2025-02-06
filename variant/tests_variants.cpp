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
    Rectangle(int width = 0, int height = 0)
        : width(width)
        , height(height)
    {
    }

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
    Circle(int radius = 0)
        : radius(radius)
    {
    }

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
    Square(int size = 0)
        : rectangle(size, size)
    {
    }

    void draw() const
    {
        cout << "Drawing a square with size " << rectangle.area() << endl;
    }

    double area() const
    {
        return rectangle.area();
    }
};

struct DrawVisitor
{
    // void operator()(const Rectangle& r) const
    // {
    //     r.draw();
    // }

    // void operator()(const Circle& c) const
    // {
    //     c.draw();
    // }

    // void operator()(const Square& s) const
    // {
    //     s.draw();
    // }

    template <typename T>
    void operator()(const T& shape) const
    {
        shape.draw();
    }
};

TEST_CASE("variant")
{
    std::variant<Rectangle, Circle, Square> shape;
    REQUIRE(std::holds_alternative<Rectangle>(shape));

    shape = Circle{10};
    shape = Square{20};

    Square& ref_square = std::get<Square>(shape);
    ref_square.draw();

    REQUIRE(shape.index() == 2);

    SECTION("visiting variants")
    {
        using Shape = std::variant<Rectangle, Circle, Square>;
        std::vector<Shape> shapes = {Rectangle{10, 20}, Circle{5}, Square{15}};

        SECTION("visitor as struct")
        {
            DrawVisitor draw_visitor;

            for (const auto& shape : shapes)
                std::visit(draw_visitor, shape);
        }

        SECTION("visitor as lambda")
        {
            for (const auto& shape : shapes)
            {
                std::visit([](const auto& s) { s.draw(); }, shape);
            }

            double area{};
            for (const auto& shape : shapes)
            {
                area += std::visit([](const auto& s) { return s.area(); }, shape);
            }
        }
    }
}