#include <tuple>
#include <string>
#include <vector>
#include <catch2/catch_test_macros.hpp>

TEST_CASE("tuples")
{
    std::tuple<int, std::string, double> tpl = {42, "hello", 3.14};

    REQUIRE(std::get<0>(tpl) == 42);
    REQUIRE(std::get<1>(tpl) == "hello");
    REQUIRE(std::get<2>(tpl) == 3.14);
}