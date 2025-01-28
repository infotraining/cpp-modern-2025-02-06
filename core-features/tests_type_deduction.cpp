#include <algorithm>
#include <catch2/catch_approx.hpp>
#include <catch2/catch_test_macros.hpp>
#include <iostream>
#include <string>
#include <vector>

TEST_CASE("auto type deduction")
{
    auto i = 42;

    static_assert(std::is_same_v<decltype(i), int>);
}