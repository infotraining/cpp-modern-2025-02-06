#include <algorithm>
#include <catch2/catch_approx.hpp>
#include <catch2/catch_test_macros.hpp>
#include <iostream>
#include <list>
#include <map>
#include <memory>
#include <numeric>
#include <string>
#include <vector>

using namespace std;

#ifdef _MSC_VER
#define __PRETTY_FUNCTION__ __FUNCSIG__
#endif

/////////////////////////////////////////////
// auto declarations

uint64_t get_number()
{
    return 42;
}

TEST_CASE("auto declarations")
{
    auto x = 42;           // x : int
    const auto y = 10.0;   // y : double
    auto z = 10.0f;        // z : float
    auto c_txt = "text";   // c_txt: const char*
    auto str = "text"s;    // str: std::string
    auto n = get_number(); // n : uint64_t

    std::vector<int> vec = {1, 2, 3, 4, 5};
    auto it = vec.begin();

    auto& vec_item = vec[2];
    vec_item = 42;
    REQUIRE(vec[2] == 42);

    auto* ptr = &x;                // int*
    const auto* ptr_to_const = &x; // const int*
    const auto const_ptr = &x;     // int* const
}

void foo(int value);

void foo(const int value)
{
    //++value;
}

template <typename T>
void deduce1(T arg)
{
    std::cout << __PRETTY_FUNCTION__ << "\n";
}

namespace Cpp20
{
    void deduce1(auto arg)
    {
        std::cout << __PRETTY_FUNCTION__ << "\n";
    }
}

template <typename T>
void deduce2(T& arg)
{
    std::cout << __PRETTY_FUNCTION__ << "\n";
}

template <typename T>
void deduce3(T&& arg)
{
    std::cout << __PRETTY_FUNCTION__ << "\n";
}

TEST_CASE("type deduction rules")
{
    int x = 10;
    const int cx = 10;
    int& ref_x = x;
    const int& cref_x = x;
    int tab[10];

    SECTION("Case 1")
    {
        deduce1(x);      // deduce1<T = int>(int arg)
        deduce1(cx);     // deduce1<T = int>(int arg) - const is removed
        deduce1(ref_x);  // deduce1<T = int>(int arg) - ref is removed
        deduce1(cref_x); // deduce1<T = int>(int arg) - const and ref is removed
        deduce1(tab);    // deduce1<T = int*>(int* arg) - array decays to pointer
        deduce1("text"); // deduce1<T = const char*>(const char*) - array decays to pointer
        deduce1(foo);    // deduce1<T = void(*)(int)>(void(*arg)(int)) - function decays to function pointer

        auto a1 = x;      // int
        auto a2 = cx;     // int - const is removed
        auto a3 = ref_x;  // int - ref is removed
        auto a4 = cref_x; // int - const and ref is removed
        auto a5 = tab;    // int* - array decays to pointer
        auto a6 = "text"; // const char*
        auto a7 = foo;    // void(*)(int) - function decays to function pointer
    }

    SECTION("Case 2")
    {
        deduce2(x);      // deduce2<T = int>(int& arg)
        deduce2(cx);     // deduce2<T = const int>(const int& arg)
        deduce2(ref_x);  // deduce2<T = int>(int& arg)
        deduce2(cref_x); // deduce2<T = const int>(const int& arg)
        deduce2(tab);    // deduce2<T = int[10]>(int(&arg)[10])
        deduce2(foo);    // deduce2<T = void(int)>(void (&arg)(int))

        auto& a1 = x;      // int&
        auto& a2 = cx;     // const int&
        auto& a3 = ref_x;  // int&
        auto& a4 = cref_x; // const int&
        auto& a5 = tab;    // int(&)[10]
        auto& a6 = foo;    // void(&)(int)
    }

    SECTION("Case 3")
    {
        deduce3(x);
        deduce3(cx);
        deduce3(ref_x);
        deduce3(cref_x);
        deduce3(tab);
        deduce3(foo);
        deduce3(string("text"));

        auto&& a1 = x;
        auto&& a2 = cx;
        auto&& a3 = ref_x;
        auto&& a4 = cref_x;
        auto&& a5 = tab;
        auto&& a6 = foo;
        auto&& a7 = string("text");
    }
}

template <typename T>
T multiply_by_2(T value)
{
    return 2 * value;
}

int func2(int a, int b)
{
    return a + b;
}

auto alt_func2(int a, int b) -> int;

auto alt_func2(int a, int b) -> int
{
    return a + b;
}

TEST_CASE("decltype")
{
    using namespace std::literals;
    const std::vector<int> vec = {1, 2, 3, 4, 5};

    auto vec_auto = vec; // std::vector<int>
    REQUIRE(vec_auto == vec);

    decltype(vec) vec_decltype; // const std::vector<int>
    REQUIRE(vec_decltype.empty());

    decltype(multiply_by_2(3.0)) result{};
    result = multiply_by_2(4.0);
    REQUIRE(result == 8.0);

    std::map<int, std::string> dict = {{1, "one"}, {2, "two"}};

    using TMappedValueRef = decltype(dict[3]); // std::string&
    REQUIRE(dict.size() == 2);

    using TFuncPtr = decltype(func2)*;

    static_assert(std::is_same_v<double, decltype(result)>);
    static_assert(std::is_same_v<decltype(func2), decltype(alt_func2)>);

    int x = 42;
}

namespace GenericCode
{
    namespace Cpp20
    {
        auto multiply(auto a, auto b)
        {
            decltype(a) x;
            decltype(b) y;

            std::cout << "auto multiply(auto a, auto b)\n";
            return a * b;
        }
    }

    namespace Cpp11
    {
        template <typename T1, typename T2>
        auto multiply(T1 a, T2 b) -> decltype(a * b)
        {
            T1 x;
            T2 y;
            std::cout << "auto multiply(auto a, auto b)\n";
        }
    }
    
    inline namespace Cpp14
    {
        template <typename T1, typename T2>
        auto multiply(T1 a, T2 b)
        {
            T1 x;
            T2 y;
            std::cout << "auto multiply(auto a, auto b)\n";
            return a * b;
        }
    }

    int multiply(int a, int b)
    {
        std::cout << "int multiply(int a, int b)";
        return a * b;
    }
}

uintmax_t global_counter = 0;

void global_func(decltype(global_counter) counter)
{
}

TEST_CASE("templates & overriding")
{
    auto result1 = GenericCode::multiply(4.0, 8.0);

    auto result2 = GenericCode::multiply(42, 2L);
}

struct NoDefaultConstructor
{
    NoDefaultConstructor() = delete;

    int foo()
    {
        return 42;
    }
};

TEST_CASE("std::declval")
{
    decltype(std::declval<NoDefaultConstructor>().foo()) var1; // int
}

template <typename T = double>
struct Values
{
    int x;
    T y;
    std::string text;
    std::vector<int> data;

    void foo()
    {
    }
};

TEST_CASE("decltype & struct members")
{
    using TMemberX = decltype(Values<double>::x);
    using TMemberY = decltype(Values<float>::y);

    Values values{42, "text"};
    using TMemberText = decltype(values.text);

    using TMemberData = decltype(std::declval<Values<>>().data);

    const Values<float>* ptr_obj = nullptr;

    using Type = decltype(ptr_obj->y);
    static_assert(std::is_same_v<Type, float>);
}

//////////////////////////////////////
// auto as a type of a function result

auto describe_eveness(int a)
{
    if (a % 2 == 0)
        return "even"s;

    const std::string odd = "odd";
    return odd;
}

auto factorial(int n)
{
    if (n == 1)
        return 1ULL;
    return factorial(n-1) * n;
}    

TEST_CASE("auto as a result type from function")
{
    auto txt = describe_eveness(13);

    auto factorial_5 = factorial(5);
}

template <typename TContainer>
decltype(auto) get_nth(TContainer& container, size_t index)
{
    return container[index];
}

TEST_CASE("auto vs. decltype(auto)")
{
    using namespace std::literals;

    std::vector<int> vec = {1, 2, 3, 4};
    REQUIRE(get_nth(vec, 3) == 4);

    std::vector words = { "one"s, "two"s, "three"s };
    get_nth(words, 1) = "dwa"s;
    REQUIRE(words[1] == "dwa"s);
}