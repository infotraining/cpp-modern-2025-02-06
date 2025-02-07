#include <algorithm>
#include <catch2/catch_approx.hpp>
#include <catch2/catch_test_macros.hpp>
#include <iostream>
#include <string>
#include <vector>

namespace Nullptr
{
    void foo(int* ptr)
    {
        if (ptr != nullptr)
        {
            std::cout << "foo(ptr is not null)\n";
        }
        else
        {
            std::cout << "foo(ptr is null)\n";
        }
    }

    void foo(int value)
    {
        std::cout << "foo(int)\n";
    }

    void foo(nullptr_t)
    {
        std::cout << "foo(nullptr_t)\n";
    }    
}

TEST_CASE("long long is a new type")
{
    long long x = -1023423423423;
    uint64_t y = 1023423423423;
    uintmax_t z = 1023423423423;
    REQUIRE(sizeof(long long) == 8);
}

TEST_CASE("nullptr")
{
    Nullptr::foo(nullptr);
    // Nullptr::foo(NULL); // should be replaced by nullptr

    int* ptr1 = nullptr;
    int* ptr2{};

    Nullptr::foo(ptr1);

    REQUIRE(ptr1 == ptr2);
    REQUIRE(ptr1 == nullptr);
    REQUIRE(ptr2 == nullptr);
}

TEST_CASE("raw string literals")
{
    std::string txt = R"raw(C:\nasz katalog\twoj plik
line1)"
line2\n)raw";

    std::cout << txt << "\n";
}

TEST_CASE("string literals")
{
    using namespace std::literals;

    std::string txt1 = "abc"; // literal: const char[4]
    std::string txt2 = "abc"s; // literal: std::string

    auto str1 = "text";  // const char*
    auto str2 = R"(\text\)"s; // std::string

    std::vector<std::string> vec = { "abc"s, "def"s }; // C++17 - std::vector<std::string>
}