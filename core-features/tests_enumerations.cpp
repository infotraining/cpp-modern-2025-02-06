#include <algorithm>
#include <catch2/catch_approx.hpp>
#include <catch2/catch_test_macros.hpp>
#include <iostream>
#include <string>
#include <vector>

enum class Coffee
{
    espresso,
    cappuccino,
    flat_white,
    americano,
    v60,
    chemex
};

TEST_CASE("enum classes")
{
    Coffee coffee = Coffee::v60;

    REQUIRE(static_cast<int>(coffee) == 4);
}