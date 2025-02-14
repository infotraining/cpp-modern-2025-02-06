#include <catch2/catch_test_macros.hpp>
#include <iostream>
#include <memory>
#include <string>
#include <vector>

using namespace std;

template <typename... Ts>
struct VariadicTemplate
{
    constexpr static size_t size = sizeof...(Ts);
};

namespace Explain
{
    template <typename T, typename... TArgs>
    std::unique_ptr<T> make_unique(TArgs&&... args)
    {
        return std::unique_ptr<T>(new T(std::forward<TArgs>(args)...));
    }
}

namespace Cpp_11_14
{
    void print()
    {
        std::cout << "\n";
    }

    template <typename THead, typename... TTail>
    void print(const THead& head, const TTail&... tail)
    {
        std::cout << head << " ";
        print(tail...); // recursive call
    }
}

template <typename THead, typename... TTail>
void print(const THead& head, const TTail&... tail)
{
    std::cout << head << " ";

    if constexpr(sizeof...(tail) > 0)
        print(tail...); // recursive call
    else
        std::cout << "\n";
}

TEST_CASE("variadic templates")
{
    VariadicTemplate<> vt0;
    static_assert(decltype(vt0)::size == 0);

    VariadicTemplate<int, double, std::string> vt1;
    static_assert(decltype(vt1)::size == 3);

    print(1, 3.44, "test", std::string("abc"));
    print(665, 42);
}