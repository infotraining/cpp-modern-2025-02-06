#include <catch2/catch_test_macros.hpp>
#include <iostream>
#include <string>
#include <vector>

using namespace std;

template <typename... TValues>
auto sum(TValues... values)
{
    return (... + values); // return ((((1 + 2) + 3) + 4) + 5); - left fold
}

template <typename... TValues>
auto print_fold(TValues... values)
{
    (..., (std::cout << values << "; ")) << "\n";
}

TEST_CASE("fold expressions")
{
    auto result = sum(1, 2, 3, 4, 5);
    REQUIRE(result == 15);

    print_fold(1, 3.14, "text", std::string("abc"));
}