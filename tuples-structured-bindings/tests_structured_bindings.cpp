#include <algorithm>
#include <catch2/catch_approx.hpp>
#include <catch2/catch_test_macros.hpp>
#include <iostream>
#include <list>
#include <map>
#include <numeric>
#include <string>
#include <tuple>
#include <vector>

std::tuple<int, int, double> calc_stats(const std::vector<int>& data)
{
    std::vector<int>::const_iterator min_pos, max_pos;
    std::tie(min_pos, max_pos) = std::minmax_element(data.begin(), data.end());

    double avg = accumulate(data.begin(), data.end(), 0.0) / data.size();

    return std::make_tuple(*min_pos, *max_pos, avg);
}

namespace SinceCpp17
{
    std::tuple<int, int, double> calc_stats(const std::vector<int>& data)
{
    auto [min_pos, max_pos] = std::minmax_element(data.begin(), data.end());

    double avg = accumulate(data.begin(), data.end(), 0.0) / data.size();

    return std::tuple(*min_pos, *max_pos, avg);
}
}

TEST_CASE("Before C++17")
{
    std::vector<int> data = {4, 42, 665, 1, 123, 13};

    int min, max;
    double avg;

    std::tie(min, max, avg) = calc_stats(data);

    REQUIRE(min == 1);
    REQUIRE(max == Catch::Approx(665));
    REQUIRE(avg == Catch::Approx(141.333));
}

TEST_CASE("Since C++17 - structured bindings")
{
    std::vector<int> data = {4, 42, 665, 1, 123, 13};

    auto [min, max, avg] = calc_stats(data);

    REQUIRE(min == 1);
    REQUIRE(max == Catch::Approx(665));
    REQUIRE(avg == Catch::Approx(141.333));
}

TEST_CASE("structured bindings use case")
{
    std::map<int, std::string> dict = { {1, "one"}, {2, "two"} };

    for(const auto& [key, value] : dict)
    {
        std::cout << key << " - " << value << "\n";
    }
}