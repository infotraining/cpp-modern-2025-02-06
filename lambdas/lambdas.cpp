#include <algorithm>
#include <catch2/catch_test_macros.hpp>
#include <iostream>
#include <string>
#include <utility>
#include <vector>
#include <functional>
#include <queue>

using namespace std;

namespace Callables
{
    auto add(int a, int b) -> int
    {
        return a + b;
    }

    struct Add
    {
        int operator()(int a, int b) const
        {
            return a + b;
        }
    };

    struct MutilplyBy
    {
        int factor;

        template <typename T>
        T operator()(T n) const
        {
            return n * factor;
        }
    };
}

TEST_CASE("callables")
{
    using namespace Callables;

    REQUIRE(add(1, 3) == 4);

    int (*ptr_fun)(int, int) = &add;
    REQUIRE(ptr_fun(6, 5) == 11);

    Add sum;
    REQUIRE(sum(4, 3) == 7);

    MutilplyBy multiply_by_5{5};
    REQUIRE(multiply_by_5(3) == 15);

    std::vector vec = {1.9, 2.9, 3.5};
    std::transform(vec.begin(), vec.end(), vec.begin(), MutilplyBy{10});
    REQUIRE(vec == std::vector{19.0, 29.0, 35.0});
}

namespace Lambdas
{
    // closure class
    struct Lambda_43547452
    {
        // Lambda_43547452() = delete;
        auto operator()(int a, int b) const { return a + b; }
    };
}

int global_seed = 42;

TEST_CASE("lambda expressions")
{
    using namespace std::literals;

    auto add = [](int a, int b)
    { return a + b; }; // lambda expression
    auto other_add = [](int a, int b)
    { return a + b; };

    static_assert(!std::is_same_v<decltype(add), decltype(other_add)>);

    SECTION("is interpreted as")
    {
        auto add = Lambdas::Lambda_43547452{}; // closure object
    }

    REQUIRE(add(1, 2) == 3);

    auto describe_number = [](int n) -> std::string
    {
        if (n % 2 == 0)
            return "even";
        return "odd"s;
    };

    auto description = describe_number(42);
    static_assert(std::is_same_v<decltype(description), std::string>);

    REQUIRE(describe_number(42) == "even");

    const std::vector<int> vec_const = []
    {
        std::vector<int> vec;
        if (global_seed > 10)
            vec.push_back(55);
        vec.push_back(665);
        return vec;
    }();
}

struct Lambda_3467576543
{
    int captured_local_value;

    auto operator()(int n) const { return n > captured_local_value; }
};

TEST_CASE("captures")
{
    SECTION("by value")
    {
        int local_value = 42;

        auto is_greater_than_42 = [local_value /*captured by value*/](int n)
        { return n > local_value; }; // Lambda_3467576543{local_value}

        SECTION("default capture by val [=]")
        {
            int a, b, c, d;

            auto is_greater_than_42 = [=](int n)
            { return n > local_value; };
        }

        REQUIRE(is_greater_than_42(43));
    }

    SECTION("by ref")
    {
        int sum = 0;

        auto accumulator = [&sum /*captured by ref*/](int n)
        { sum += n; };

        SECTION("default capture by ref [&]")
        {
            auto accumulator = [&](int n)
            { sum += n; };
        }

        accumulator(10);
        accumulator(5);

        REQUIRE(sum == 15);
    }

    std::unique_ptr<std::string> outside_str;

    SECTION("by move")
    {
        std::unique_ptr<std::string> str_on_heap = std::make_unique<std::string>("text");

        auto printer = [str = std::move(str_on_heap)]
        { std::cout << *str << "\n"; };

        printer();
        printer();
        printer();
    }

    SECTION("capture many vars")
    {
        uint64_t even_counter{};
        uint64_t odd_counter{};
        std::string prefix = "item";

        std::vector<int> vec = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};

        auto even_odd_counter =
            //[&even_counter, &odd_counter, prefix](int n) {
            [&, prefix](int n)
        {
            std::cout << prefix << ": " << n << "\n";
            if (n % 2 == 0)
                ++even_counter;
            else
                ++odd_counter;
        };

        std::for_each(vec.begin(), vec.end(), even_odd_counter);

        REQUIRE(odd_counter == 5);
        REQUIRE(even_counter == 5);
    }
}

TEST_CASE("lambda + algorithms")
{
    std::vector<std::string> words = {"one", "two", "fourty-two", "sixty-six", "zero"};

    std::sort(words.begin(), words.end(), [](const std::string& a, const std::string& b)
        { return a.size() < b.size(); });

    std::sort(words.begin(), words.end(), [](const std::string& a, const std::string& b)
        { return a.size() < b.size(); });

    for (const auto& w : words)
        std::cout << w << "\n";

    auto cmp_by_size_desc = [](const auto& a, const auto& b)
    { return a.size() > b.size(); };
    std::sort(words.begin(), words.end(), cmp_by_size_desc);
}

struct Gadget
{
    int id{42};
    std::string name{"Gadget"};

    void use() const
    {
        auto printer = [this]
        { std::cout << "Gadget: " << id << ", " << name << "\n"; };
        printer();
    }

    void safe_use()
    {

        auto printer = [&self = std::as_const(*this)]
        { 
            std::cout << "const Gadget: " << self.id << ", " << self.name << "\n"; 
            self.use();
        };
        printer();
    }
};

TEST_CASE("capturing this")
{
    Gadget g;

    g.use();
}

auto create_generator(int seed)
{
    return [seed]() mutable
    { return ++seed; };
}

TEST_CASE("mutable lambda")
{
    int n = 0;

    auto generator = [n]() mutable
    { return ++n; };

    REQUIRE(generator() == 1);
    REQUIRE(generator() == 2);
    REQUIRE(generator() == 3);
    REQUIRE(generator() == 4);

    auto gen1 = create_generator(42);
    REQUIRE(gen1() == 43);

    std::vector<int> vec(10);

    std::generate_n(vec.begin(), vec.size(), create_generator(1000));
    REQUIRE(vec == std::vector{1001, 1002, 1003, 1004, 1005, 1006, 1007, 1008, 1009, 1010});
}

struct Lambda_427635765
{
    int factor_captured;

    template <typename T>
    auto operator()(const T& n) const
    {
        {
            return n * factor_captured;
        }
    }
};

TEST_CASE("generic lambdas")
{
    std::vector vec = {1.5, 2.3, 3.9};

    int factor = 10;
    std::transform(vec.begin(), vec.end(), vec.begin(), [factor](const auto& n)
        { return n * factor; });
    REQUIRE(vec == std::vector{15.0, 23.0, 39.0});
}

TEST_CASE("as_const")
{
    std::string prefix = "item";

    auto printer = [&prefix = std::as_const(prefix)](const auto& item)
    { std::cout << prefix << ": " << item << "\n"; };

    printer(53);
    printer("forty-two");
}

/////////////////////////////////////////////////////
// passing lambdas as function params
namespace Explain
{
    template <typename TContainer, typename TPredicate>
    auto find_if(const TContainer& container, TPredicate pred)
    {
        using std::begin;
        using std::end;

        auto it = begin(container);
        auto it_end = end(container);
        for (; it != it_end; ++it)
            if (pred(*it))
                return it;

        return it;
    }
}

TEST_CASE("passing lambdas")
{
    std::vector<int> vec = {1, 2, 3, 42, 5};

    auto pos_gt_10 = Explain::find_if(vec, [](int n) { return n > 10; });
    REQUIRE(*pos_gt_10 == 42);
}

struct Fan
{
    void on()
    {
        std::cout << "Fan::on()\n";
    }

    void off()
    {
        std::cout << "Fan::off()\n";
    }
};

TEST_CASE("storing lambdas")
{
    SECTION("auto - prefered")
    {
        auto cmp_deref = [](int* a, int* b) { return *a < *b; };

        int x = 10;
        int y = 42;

        REQUIRE(cmp_deref(&x, &y) == true);
    }

    SECTION("ptr to function - for non-capturing lambdas")
    {
        bool (*ptr_fun)(int*, int*) = nullptr;

        ptr_fun = [](int* a, int* b) { return *a < *b; }; // OK because capture clause is empty
        ptr_fun = [](int* a, int* b) { return *a > *b; };

        int x = 10;
        int y = 42;

        REQUIRE(ptr_fun(&x, &y) == false);

        using Callback_CStyle =  void(*)(double);
        Callback_CStyle on_temperature_change;

        on_temperature_change = [](double temp) { std::cout << "Current temp: " << temp << "\n"; };

        on_temperature_change(21.2);
        on_temperature_change(21.5);
        on_temperature_change(21.3);
    }

    SECTION("std::function")
    {
        std::function<void(double)> on_temperature_change;

        on_temperature_change = [](double temp) { std::cout << "Current temp: " << temp << "\n"; };
        on_temperature_change(24.5);

        Fan fan;

        on_temperature_change = [&fan](double temp) { if (temp > 25.0) fan.on(); };
        on_temperature_change(28.4);
    }
}

struct TaskQueue
{
    using Task = std::function<void()>;

    std::queue<Task> tasks_;

    template<typename TTask>
    void submit(TTask&& task)
    {
        tasks_.push(std::forward<TTask>(task));
    }

    void run()
    {
        while(!tasks_.empty())
        {
            Task task = tasks_.front();
            task();
            tasks_.pop();
        }
    }
};

void start()
{
    std::cout << "Start...\n";
}

TEST_CASE("TaskQueue")
{
    auto fan = std::make_shared<Fan>();

    TaskQueue q;

    q.submit(&start);

    auto turn_on_fan = [fan]() { fan->on(); };
    q.submit(turn_on_fan);
    q.submit([fan]() { fan->off(); });
    q.submit([]{ std::cout << "Stop...\n"; });

    q.run();
}