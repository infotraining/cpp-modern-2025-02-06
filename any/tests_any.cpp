#include <algorithm>
#include <any>
#include <catch2/catch_test_macros.hpp>
#include <iostream>
#include <numeric>
#include <string>
#include <vector>
#include <map>

using namespace std;

class Dictionary
{
    std::map<std::string, std::any> data_;
public:
    template <typename TValue>
    void insert(std::string key, TValue&& value)
    {
        data_.emplace(std::move(key), std::forward<TValue>(value));
    }

    template <typename TValue>
    TValue get(const std::string& key) const
    {
        return std::any_cast<TValue>(data_.at(key));
    }
};

struct TempMonitor
{
    int id;
};

struct Observer
{
    virtual ~Observer() = default;
    virtual void notify(std::any sender) = 0;
};


struct TempObserver : Observer
{
    void notify(std::any sender) override
    {
        TempMonitor* monitor = std::any_cast<TempMonitor*>(sender);
        std::cout << monitor->id << "\n";
    }
};

TEST_CASE("any - safe void*")
{
    std::any anything;
    REQUIRE_FALSE(anything.has_value());

    anything = 42;
    anything = 3.14;
    anything = std::string{"abc"};
    anything = std::vector{1, 2, 3};

    auto vec = std::any_cast<std::vector<int>>(anything);
    REQUIRE(vec == std::vector{1, 2, 3});
    REQUIRE_THROWS_AS(std::any_cast<int>(anything), std::bad_any_cast);

    std::vector<int>* ptr_vec = std::any_cast<std::vector<int>>(&anything);
    REQUIRE(ptr_vec != nullptr);

    Dictionary dict;

    dict.insert("int", 42);
    dict.insert("double", 3.14);
    dict.insert("string", std::string{"abc"});
    dict.insert("vector", std::vector{1, 2, 3});

    REQUIRE(dict.get<int>("int") == 42);
    REQUIRE(dict.get<double>("double") == 3.14);
    REQUIRE(dict.get<std::string>("string") == "abc");
    REQUIRE(dict.get<std::vector<int>>("vector") == std::vector{1, 2, 3});
}