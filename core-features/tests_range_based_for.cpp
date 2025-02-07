#include <algorithm>
#include <catch2/catch_approx.hpp>
#include <catch2/catch_test_macros.hpp>
#include <iostream>
#include <list>
#include <string>
#include <vector>

namespace Explain
{
    template <typename T>
    auto begin(T& container)
    {
        return container.begin();
    }

    template <typename T>
    auto end(T& container)
    {
        return container.end();
    }

    template <typename T, size_t N>
    auto begin(T (&tab)[N])
    {
        return tab;
    }

    template <typename T, size_t N>
    auto end(T (&tab)[N])
    {
        return tab + N;
    }
}

TEST_CASE("looping with auto iterator")
{
    SECTION("containers")
    {
        // std::vector<int> vec = {1, 2, 3, 4, 5, 6, 42, 999};
        int vec[10] = {1, 2, 3, 4, 5, 6, 42, 999};

        for (auto it = std::begin(vec); it != std::end(vec); ++it)
        {
            *it += 2;
        }

        for (auto it = std::cbegin(vec); it != std::cend(vec); ++it)
        {
            std::cout << *it << " ";
        }
        std::cout << "\n";
    }
}

TEST_CASE("range based for")
{
    std::vector<int> vec = {1, 2, 3, 4, 5};

    for (auto& i : vec)
    {
        i *= 2;
    }

    SECTION("how it works")
    {
        for (const auto& i : vec)
        {
            std::cout << i << " ";
        }
        std::cout << "\n";

        SECTION("is interpreted as")
        {
            auto&& container = vec;
            auto __begin = vec.begin(); // begin(vec) is also tried
            auto __end = vec.end();     // end(vec) is also tried
            for (auto it = __begin; it != __end; ++it)
            {
                const auto& i = *it;
                std::cout << i << " ";
            }
        }
    }

    SECTION("for container of pointers")
    {
        std::vector<std::string> words = {"one", "forty-two"};
        std::vector<std::string*> word_ptrs = {&words[0], &words[1]};

        for (const auto& w : words)
            std::cout << w << " ";
        std::cout << "\n";

        for (auto ptr : word_ptrs)
            std::cout << ptr->size() << " ";
        std::cout << "\n";
    }
}

TEST_CASE("range-based for works with")
{
    SECTION("std containers")
    {
        std::vector<int> vec = {1, 2, 3, 4, 5};

        for (const auto& i : vec)
        {
            std::cout << i << " ";
        }
        std::cout << "\n";
    }

    SECTION("native arrays")
    {
        int native_tab[10] = {1, 2, 3, 4};

        for (const auto& item : native_tab)
        {
            std::cout << item << " ";
        }
        std::cout << "\n";
    }

    SECTION("initializer lists")
    {
        auto lst = {1, 2, 3, 4, 5};

        for (const auto& item : lst)
        {
            std::cout << item << " ";
        }
        std::cout << "\n";

        for (const auto& item : {1, 2, 3})
            std::cout << item << " ";
        std::cout << "\n";
    }
}

struct Point
{
    int x, y;
};

struct Triangle
{
    using iterator = Point*; // typedef Point* iterator;
    using const_iterator = const Point*;

    Point vertices[3];

    iterator begin() { return vertices; }
    iterator end() { return vertices + 3; }

    const_iterator begin() const { return vertices; }
    const_iterator end() const { return vertices + 3; }
};

namespace AlternativeTake
{
    struct Triangle
    {
        Point vertices[3];
    };

    Point* begin(Triangle& t)
    {
        return t.vertices;
    }

    Point* end(Triangle& t)
    {
        return t.vertices + 3;
    }

    const Point* begin(const Triangle& t)
    {
        return t.vertices;
    }

    const Point* end(const Triangle& t)
    {
        return t.vertices + 3;
    }
}

TEST_CASE("custom types & range-based for")
{
    const Triangle t{Point{1, 0}, Point{3, 3}, Point{9, 5}};

    for (const auto& vertex : t)
    {
        std::cout << "Point(x: " << vertex.x << ", y: " << vertex.y << ") ";
    }
    std::cout << "\n";
}