#include <array>
#include <catch2/catch_test_macros.hpp>
#include <iostream>
#include <map>
#include <string>
#include <vector>
#include <cmath>

using namespace std;

///////////////////////////////////////////////////////////
// template aliases

template <typename TValue>
using StringKeyMap = std::map<std::string, TValue>;

template <std::size_t N>
using StringArray = std::array<std::string, N>;

TEST_CASE("template aliases")
{
    StringKeyMap<int> dictionary = {{"one", 1}, {"two", 2}};

    StringArray<1024> string_buffer{};
}

////////////////////////////////////////////////////////////
// template  variables

template <typename T>
constexpr T pi_v(3.1415926535897932385);

constexpr auto pi = pi_v<double>;

TEST_CASE("pi as template variable")
{
    std::cout << "Pi: " << pi_v<double> << "\n";

    std::cout << "Pi: " << pi_v<float> << "\n";

    std::cout << "Pi: " << pi << "\n";
}

/////////////////////////////////////////////////////////////
// type traits
namespace Explain
{

    bool is_same_value(int a, int b)
    {
        return a == b;
    }

    // is_same - type trait
    template <typename T1, typename T2>
    struct is_same
    {
        constexpr static bool value = false;
    };

    template <typename T>
    struct is_same<T, T>
    {
        constexpr static bool value = true;
    };

    template <typename T1, typename T2>
    constexpr bool is_same_v = is_same<T1, T2>::value;

    // remove_const - type trait
    template <typename T>
    struct remove_const
    {
        using type = T;
    };

    template <typename T>
    struct remove_const<const T>
    {
        using type = T;
    };

    template <typename T>
    using remove_const_t = typename remove_const<T>::type;
}

static_assert(Explain::is_same_v<int, int>);
static_assert(!Explain::is_same<int, const int>::value);
static_assert(!Explain::is_same_v<int, double>);

template <typename T>
auto create_filled_vector(size_t size, T& value)
{
    std::vector<Explain::remove_const_t<T>> vec;
    for (size_t i = 0; i < size; ++i)
        vec.push_back(value);
    return vec;
}

TEST_CASE("type traits & templates")
{
    const int x = 10;
    auto vec = create_filled_vector(3, x);

    REQUIRE(vec == std::vector{10, 10, 10});
}

template <typename T>
bool is_power_of_2(T value)
{
    // since C++1
    if constexpr(std::is_integral_v<T>)
    {
        return value > 0 && (value & (value - 1)) == 0;
    }
    else
    {
        int exponent;
        const T mantissa = std::frexp(value, &exponent);
        return mantissa == static_cast<T>(0.5);
    }
}

TEST_CASE("is_power_of_2")
{
    REQUIRE(is_power_of_2(8.0));
    REQUIRE(is_power_of_2(16L));
    REQUIRE(is_power_of_2(1024UL));
    REQUIRE_FALSE(is_power_of_2(3));

    static_assert(std::is_same_v<std::add_pointer_t<std::remove_const_t<std::remove_pointer_t<const int*>>>, int*>);
    static_assert(std::is_const_v<int* const>);
    static_assert(std::is_const_v<const int* const>);
}