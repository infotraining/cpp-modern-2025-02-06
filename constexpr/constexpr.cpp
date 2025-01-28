#include <catch2/catch_test_macros.hpp>
#include <iostream>
#include <string>
#include <vector>

using namespace std;

TEST_CASE("constexpr")
{
    constexpr int i = 42;
    static_assert(i == 42);
}