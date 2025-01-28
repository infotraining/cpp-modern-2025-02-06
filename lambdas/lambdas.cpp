#include <catch2/catch_test_macros.hpp>
#include <iostream>
#include <string>
#include <vector>

using namespace std;

TEST_CASE("lambda expressions")
{
    auto add = [](int a, int b) { return a + b; };

    REQUIRE(add(1, 2) == 3);
}