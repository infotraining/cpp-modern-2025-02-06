#include <algorithm>
#include <catch2/catch_approx.hpp>
#include <catch2/catch_test_macros.hpp>
#include <iostream>
#include <string>
#include <vector>

TEST_CASE("range based for")
{
    std::vector<int> vec = {1, 2, 3, 4, 5};

    for (int i : vec)
    {
        std::cout << i << " ";
    }
    std::cout << "\n";
}