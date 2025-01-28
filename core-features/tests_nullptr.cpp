#include <algorithm>
#include <catch2/catch_approx.hpp>
#include <catch2/catch_test_macros.hpp>
#include <iostream>
#include <string>
#include <vector>

void foo(int* ptr)
{
    if (ptr)
    {
        std::cout << "foo(ptr is not null)\n";
    }
    else
    {
        std::cout << "foo(ptr is null)\n";
    }
}

TEST_CASE("nullptr")
{
    foo(NULL);
}