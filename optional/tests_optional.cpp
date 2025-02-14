#include <algorithm>
#include <array>
#include <atomic>
#include <catch2/catch_approx.hpp>
#include <catch2/catch_test_macros.hpp>
#include <charconv>
#include <iostream>
#include <numeric>
#include <optional>
#include <string>
#include <vector>

using namespace std;

TEST_CASE("optional")
{
    std::optional<int> opt_int;
    REQUIRE(opt_int.has_value() == false);

    opt_int = 42;
    REQUIRE(opt_int.has_value());

    if (opt_int)
    {
        std::cout << "opt_int: " << *opt_int << "\n";
    }

    REQUIRE(opt_int.value() == 42);

    opt_int = std::nullopt;
    REQUIRE_THROWS_AS(opt_int.value(), std::bad_optional_access);
}

TEST_CASE("optional + CTAD")
{
    std::optional opt_int = 42;
    REQUIRE(opt_int.value() == 42);
}

std::optional<const char*> maybe_getenv(const char* n)
{
    if (const char* env_var = std::getenv(n))
        return env_var;
    else
        return std::nullopt;
}

TEST_CASE("maybe_getenv")
{
    std::cout << maybe_getenv("NOTPATH").value_or("(NOT FOUND)") << "\n";
}