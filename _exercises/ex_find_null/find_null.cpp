#include <cassert>
#include <catch2/catch_test_macros.hpp>
#include <iostream>
#include <iterator>
#include <memory>
#include <vector>

namespace Custom
{
    struct Container
    {
        std::vector<int> items;

    };
    
    auto begin(Container& c) { return c.items.begin(); }
    
    auto end(Container& c) { return c.items.end(); }

    void use(Container& c)
    {
        std::cout << "Container::use()" << "\n";
    }
}

template <typename T>
auto find_null(T& cont)
{
    using std::begin;
    using std::end;
    auto it = begin(cont);
    for (; it != end(cont); it++)
    {
        if (*it == nullptr)
        {
            return it;
        }
    }

    return it;
}

// ADL - Argument Dependent Lookup
TEST_CASE("custom")
{
    Custom::Container c{};
    Custom::use(c);

    use(c); // ADL
}

namespace Alternative
{
    template <typename T>
    auto find_null(T& container)
    {
        using std::begin;
        using std::end;
        //return std::find_if(begin(container), end(container), [](const auto& ptr) { return ptr == nullptr; });
        return std::find(begin(container), end(container), nullptr);
    }
}

TEST_CASE("find_null description")
{
    using namespace std;

    SECTION("finds first null pointer in a std container of raw pointers")
    {
        vector<int*> ptrs = {new int{9}, new int{10}, nullptr, new int{20}, nullptr, new int{23}};

        vector<int*>::iterator first_null_appearence = Alternative::find_null(ptrs);

        REQUIRE(distance(ptrs.begin(), first_null_appearence) == 2);

        // clean-up
        for (const auto* ptr : ptrs)
            delete ptr;
    }

    SECTION("finds first null pointer in an array of raw pointers")
    {
        int* ptrs[] = {new int{9}, new int{10}, NULL, new int{20}, nullptr, new int{23}};

        auto first_null_appearence = Alternative::find_null(ptrs);

        REQUIRE(distance(begin(ptrs), first_null_appearence) == 2);

        // clean-up
        for (const auto* ptr : ptrs)
            delete ptr;
    }

    SECTION("finds first empty shared_ptr in a initializer-list of shared_ptrs")
    {
        auto il = {make_shared<int>(10), shared_ptr<int>{}, make_shared<int>(3)};

        auto first_null_appearence = find_null(il);

        REQUIRE(distance(il.begin(), first_null_appearence) == 1);
    }

    SECTION("finds first empty unique_ptr in a container of unique_ptrs")
    {
        vector<unique_ptr<int>> vec_ptr;
        vec_ptr.push_back(std::unique_ptr<int>(new int(10)));
        vec_ptr.push_back(nullptr);
        vec_ptr.push_back(std::unique_ptr<int>(new int(20)));

        auto where_null = find_null(vec_ptr);

        REQUIRE(distance(vec_ptr.begin(), where_null) == 1);
    }

    SECTION("when all pointers are valid returns iterator which equals end()")
    {
        auto il = {make_shared<int>(10), shared_ptr<int>{new int(5)}, make_shared<int>(3)};

        REQUIRE(find_null(il) == il.end());
    }
}