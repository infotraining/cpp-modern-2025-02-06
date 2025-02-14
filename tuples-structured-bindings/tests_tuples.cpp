#include <tuple>
#include <string>
#include <vector>
#include <catch2/catch_test_macros.hpp>

using namespace std::literals;

TEST_CASE("tuples")
{
    std::tuple<int, std::string, double> tpl1 = {42, "hello", 3.14};

    REQUIRE(std::get<0>(tpl1) == 42);
    REQUIRE(std::get<1>(tpl1) == "hello");
    REQUIRE(std::get<2>(tpl1) == 3.14);

    // CTAD
    std::tuple tpl2{665, "text", 2.71};
    REQUIRE(std::get<0>(tpl2) == 665);

    tpl1 = tpl2;  // ok
    REQUIRE(tpl1 == std::tuple{665, "text"s, 2.71});
}

TEST_CASE("reference tuples")
{
    int x = 42;
    double y = 3.14;

    std::tuple<int&, double&> tpl_ref{x, y};
    REQUIRE(std::get<0>(tpl_ref) == 42);

    tpl_ref = std::tuple{665, 2.71}; // assing to x & y
    REQUIRE(x == 665);
    REQUIRE(y == 2.71);

    std::tie(x, y) = std::tuple{777, 1.22};
}