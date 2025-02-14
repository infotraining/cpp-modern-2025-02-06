#include <catch2/catch_test_macros.hpp>
#include <iostream>
#include <string>
#include <vector>

using namespace std;

namespace Cpp_11
{
    constexpr int factorial(int n)
    {
        return (n == 0) ? 1 : n * factorial(n - 1);
    }
}

constexpr int factorial(int n)
{
    if (n <= 1)
        return 1;
    return n * factorial(n - 1);
}

template <size_t N>
constexpr std::array<int, N> create_factorial_lookup_table()
{
    std::array<int, N> result{};
 
    for(int n = 0; n < N; ++n)
        result[n] = factorial(n);

    return result;
}

TEST_CASE("constexpr")
{
    constexpr int i = 42;
    static_assert(i == 42);

    static_assert(Cpp_11::factorial(3) == 6);
    int tab[factorial(5)]; 

    int x = 3;
    REQUIRE(Cpp_11::factorial(x) == 6);

    constexpr auto factorial_lookup_table = create_factorial_lookup_table<10>();

    static_assert(factorial_lookup_table[9] == 362880);
}