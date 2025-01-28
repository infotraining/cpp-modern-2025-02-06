#include <algorithm>
#include <catch2/catch_approx.hpp>
#include <catch2/catch_test_macros.hpp>
#include <iostream>
#include <string>
#include <vector>

struct Aggregate
{
    int a;
    double b;
    std::string c;
};

class Class
{
public:
    int a;
    double b;
    std::string c;

    Class(int a, double b, std::string c) : a(a), b(b), c(c) {}
};

TEST_CASE("initialization syntax before C++11")
{
    int a1 = 42;
    int a2(42);
    int a3 = int(42);

    REQUIRE(a1 == 42);
    REQUIRE(a2 == 42);
    REQUIRE(a3 == 42);
}