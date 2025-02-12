#include "gadget.hpp"

#include <catch2/catch_test_macros.hpp>
#include <utility>
#include <type_traits>

// #define MSVC

#if !defined(__PRETTY_FUNCTION__) && !defined(__GNUC__)
#define __PRETTY_FUNCTION__ __FUNCSIG__
#endif

////////////////////////////////////////////////////////
///  PERFECT FORWARDING

void have_fun(Gadget& g)
{
    std::cout << __PRETTY_FUNCTION__ << std::endl;
    g.use();
}

void have_fun(const Gadget& cg)
{
    std::cout << __PRETTY_FUNCTION__ << std::endl;
    cg.use();
}

void have_fun(Gadget&& g)
{
    std::cout << __PRETTY_FUNCTION__ << std::endl;
    g.use();
}

TEST_CASE("have_fun with Gadget")
{
    Gadget g{1, "gadget#1"};
    const Gadget cg{2, "const-gadget#2"};

    have_fun(g);                        // have_fun(Gadget&);
    have_fun(cg);                       // have_fun(const Gadget&)
    have_fun(Gadget{3, "temp-gadget"}); // have_fun(Gadget&&)
    have_fun(std::move(g));             // have_fun(Gadget&&)
}

namespace HandwrittenForwarding
{
    void use(Gadget& g)
    {
        have_fun(g);
    }

    void use(const Gadget& g)
    {
        have_fun(g);
    }

    void use(Gadget&& g) // g - lvalue of type rvalue-ref to Gadget
    {
        have_fun(std::move(g));
    }
}

namespace PerfectForwarding
{
    template <typename T>
    void use(T&& g) // T&& - universal reference
    {
        have_fun(std::forward<T>(g));
    }
}

// TEST_CASE("4---")
// {
//     std::cout << "\n--------------------------\n\n";
// }

TEST_CASE("custom forwarding")
{
    using PerfectForwarding::use;

    Gadget g{1, "gadget"};
    const Gadget cg{2, "const-gadget"};

    use(g);
    use(cg);
    use<Gadget&&>(Gadget{3, "temp-gadget"});
}

template <typename T>
struct MyVector
{
public:
    // void push_back(const T& t)
    // {
    //     std::cout << "push(const T& t)" << std::endl;
    // }

    // void push_back(T&& t)
    // {
    //     std::cout << "push(T&& t)" << std::endl;
    // }

    template <typename U>
    void push_back(U&& t) // universal reference
    {
        std::cout << "push(T&& t)" << std::endl;
    }
};

TEST_CASE("auto&&")
{
    Gadget g{1, "gadget"};

    auto&& universal_ref1 = g; // g is lvalue -> universal_ref1 is lvalue&: Gadget&

    auto&& universal_ref2 = Gadget{2, "temp-gadget"}; // Gadget{2, "temp-gadget"} is rvalue -> universal_ref2 is rvalue&: Gadget&&
}

///////////////////////////////////////////////////
///  PERFECT FORWARDING & std::exchange

namespace Explain
{
    // Gadget exchange(Gadget& g, const Gadget& new_g)
    // {
    //     Gadget old_g = std::move(g); // move g to old_g, call move constructor: Gadget(Gadget&&)
    //     g = new_g;
    //     return old_g;
    // }

    template<typename T, typename TNewValue = T>
    T exchange(T& g, TNewValue&& new_g) noexcept(std::is_nothrow_move_constructible_v<T> && std::is_nothrow_move_assignable_v<T>)
    {
        T old_g = std::move(g); // move g to old_g, call move constructor: Gadget(Gadget&&)
        g = std::forward<TNewValue>(new_g);
        return old_g;
    }
}

TEST_CASE("std::exchange")
{
    auto ptr_exchange_fun = Explain::exchange<Gadget>;

    Gadget g{1, "gadget"};
    Gadget new_g{2, "new-gadget"};

    Gadget old_g = std::exchange(g, Gadget{665, "temp-gadget"});

    g.use();
    old_g.use();
}

void foobar(int x, const std::string& s)
{
    std::cout << "foobar(" << x << ", " << s << ")\n";
}

template<typename F, typename TArg1, typename TArg2>
decltype(auto) call_wrapper(F f, TArg1&& arg1, TArg2&& arg2)
{
    std::cout << "Log: " << " calling f!!!\n";
    return f(std::forward<TArg1>(arg1), std::forward<TArg2>(arg2));
}

TEST_CASE("call_wrapper")
{
    int x = 42;
    call_wrapper(foobar, x, std::string("hello"));
}

void just_use_as_const(const std::vector<int>& vec)
{
    std::cout << "size of vector: " << vec.size() << "\n";
}

std::vector<std::vector<int>> global_vec{};

template <typename T>
void insert_into_global(T&& vec)
{
    global_vec.push_back(std::forward<T>(vec));
}

void never_throws(int a) noexcept
{
}

TEST_CASE("passing objects")
{
    std::vector<int> local_vec = {1, 2, 3};
    insert_into_global(local_vec);
    insert_into_global(std::vector{645, 765, 34, 66});

    void (*fptr)(int) noexcept = &never_throws;
}
