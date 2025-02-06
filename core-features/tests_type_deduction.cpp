#include <catch2/catch_approx.hpp>
#include <catch2/catch_test_macros.hpp>

#include <algorithm>
#include <iostream>
#include <memory>
#include <numeric>
#include <string>
#include <vector>
#include <list>

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
    auto x = 42;     // x : int
    const auto y = 10.0;   // y : double
    auto z = 10.0f;  // z : float
    auto c_txt = "text"; // c_txt: const char* 
    auto str = "text"s; // str: std::string
    auto n = get_number(); // n : uint64_t

    std::vector<int> vec = {1, 2, 3, 4, 5};
    auto it = vec.begin();

    auto& vec_item = vec[2];
    vec_item = 42;
    REQUIRE(vec[2] == 42);

    auto* ptr = &x; // int*
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
        deduce1(x);   // deduce1<T = int>(int arg)
        deduce1(cx);  // deduce1<T = int>(int arg) - const is removed
        deduce1(ref_x);  // deduce1<T = int>(int arg) - ref is removed
        deduce1(cref_x); // deduce1<T = int>(int arg) - const and ref is removed
        deduce1(tab);    // deduce1<T = int*>(int* arg) - array decays to pointer
        deduce1("text"); // deduce1<T = const char*>(const char*) - array decays to pointer
        deduce1(foo);    // deduce1<T = void(*)(int)>(void(*arg)(int)) - function decays to function pointer 

        auto a1 = x;       // int
        auto a2 = cx;      // int - const is removed
        auto a3 = ref_x;   // int - ref is removed
        auto a4 = cref_x;  // int - const and ref is removed
        auto a5 = tab;     // int* - array decays to pointer
        auto a6 = "text";  // const char*
        auto a7 = foo;     // void(*)(int) - function decays to function pointer 
    }

    SECTION("Case 2")
    {
        deduce2(x);         // deduce2<T = int>(int& arg)
        deduce2(cx);        // deduce2<T = const int>(const int& arg)
        deduce2(ref_x);     // deduce2<T = int>(int& arg)
        deduce2(cref_x);    // deduce2<T = const int>(const int& arg)
        deduce2(tab);       // deduce2<T = int[10]>(int(&arg)[10])
        deduce2(foo);       // deduce2<T = void(int)>(void (&arg)(int))

        auto& a1 = x;       // int&
        auto& a2 = cx;      // const int&
        auto& a3 = ref_x;   // int&
        auto& a4 = cref_x;  // const int&
        auto& a5 = tab;     // int(&)[10]
        auto& a6 = foo;     // void(&)(int)
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

TEST_CASE("decltype(auto)")
{ 
}
