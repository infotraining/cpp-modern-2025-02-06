#include <algorithm>
#include <catch2/catch_approx.hpp>
#include <catch2/catch_test_macros.hpp>
#include <iostream>
#include <string>
#include <vector>

struct Aggregate
{
    int a;
    double b;
    std::string c;
};

class Class
{
public:
    int a{};
    double b{};
    std::string c;

    Class()
    {
    }

    Class(int a, double b, std::string c)
        : a{a}
        , b{b}
        , c{c}
    {
    }
};

TEST_CASE("initialization syntax before C++11")
{
    SECTION("primitive types")
    {
        int a1 = 42;
        int a2(42);
        // int a_evil(); // most vexing parse
        int a3 = int(42);

        REQUIRE(a1 == 42);
        REQUIRE(a2 == 42);
        REQUIRE(a3 == 42);
    }

    SECTION("aggregates")
    {
        Aggregate agg1{1, 3.14, "text"};
        Aggregate agg2{1};
        Aggregate agg3 = {1, 3.14};
    }

    SECTION("classes with constructor")
    {
        Class c1(1, 3.14, "text");
    }

    SECTION("containers")
    {
        std::vector<int> vec;
        vec.push_back(42);
        vec.push_back(665);
    }
}

TEST_CASE("uniform init syntax")
{
    SECTION("primitive types")
    {
        int a1 = 42;
        int a2{42};
        int* ptr1{&a1};
        int a3 = int{42};
        int a4{};    // int a3 = 0;
        int* ptr2{}; // int* ptr2 = nullptr;

        REQUIRE(a1 == 42);
        REQUIRE(a2 == 42);
        REQUIRE(a3 == 42);
        REQUIRE(a4 == 0);
        REQUIRE(ptr2 == nullptr);
    }

    SECTION("aggregates")
    {
        Aggregate agg1{1, 3.14, "text"};
        Aggregate agg2{1};
        Aggregate agg3 = {1, 3.14};
    }

    SECTION("classes with constructor")
    {
        Class c1{1, 3.14, "text"}; // constructor call
    }

    SECTION("containers")
    {
        std::vector<int> vec{42, 665};
    }
}

uint64_t get_value()
{
    return 642;
}

TEST_CASE("uniform init - implicit narrowing conversion is an error")
{
    uint64_t value{get_value()};
}

//////////////////////////////////////////////
// initializer_list

template <typename T>
void print_items(std::initializer_list<T> lst)
{
    std::cout << "List of " << lst.size() << " items: ";

    for (const auto& item : lst)
        std::cout << item << " ";
    std::cout << "\n";
}

TEST_CASE("initializer lists")
{
    using namespace std::literals;

    auto lst = {1, 2, 3}; // std::initializer_list
    print_items(lst);

    print_items({"one"s, "two"s});
}

class ContainerInt
{
public:
    ContainerInt(size_t size, int value)
    {
        size_ = size;
        items_ = new int[size_];

        std::fill_n(items_, size_, value);
    }

    ContainerInt(std::initializer_list<int> values)
    {
        size_ = values.size();
        items_ = new int[size_];

        std::copy(values.begin(), values.end(), items_);
    }

    ~ContainerInt()
    {
        delete[] items_;
    }

    int& operator[](size_t index)
    {
        return items_[index];
    }

    const int& operator[](size_t index) const
    {
        return items_[index];
    }

    size_t size() const
    {
        return size_;
    }

private:
    size_t size_;
    int* items_;
};

TEST_CASE("custom container")
{
    ContainerInt container = {1, 2, 3, 4, 5, 6, 7};
    REQUIRE(container[3] == 4);

    ContainerInt other_container(10, 1);
    REQUIRE(other_container.size() == 10);

    ContainerInt another_container{10, 1};
    REQUIRE(another_container.size() == 2);

    SECTION("beware () & {} for vector")
    {
        std::vector<int> vec1(5, 1);
        REQUIRE(vec1 == std::vector{1, 1, 1, 1, 1});

        std::vector<int> vec2{5, 1};
        REQUIRE(vec2 == std::vector{5, 1});
    }
}

class XAggregate
{
public:
    int a, b, c;
};

struct XClassWithCtor
{
    int a, b, c;

    XClassWithCtor(int a = 0, int b = 0, int c = 0)
        : a{a}
        , b{b}
        , c{c}
    {
    }
};

TEST_CASE("question")
{
    XAggregate x1{1, 2, 3};
    XAggregate x2{};
    XAggregate x3{1, 2};

    XClassWithCtor x4{1, 2, 3};
    XClassWithCtor x5(1, 2, 3);
    XClassWithCtor x6{};
    XClassWithCtor x7{1, 2};
    XClassWithCtor x8(1, 2);
}