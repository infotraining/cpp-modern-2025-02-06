#include <algorithm>
#include <catch2/catch_approx.hpp>
#include <catch2/catch_test_macros.hpp>
#include <iostream>
#include <map>
#include <span>
#include <string>
#include <vector>

// classic enums
enum State : uint16_t
{
    open,
    closed,
    faulty = 665
};

static_assert(sizeof(State) == 2);

TEST_CASE("classic enums")
{
    State s = open;
    s = static_cast<State>(1);

    int int_value = s;

    std::underlying_type_t<State> s_value = s;
    REQUIRE(s_value == 665);
}

enum class Coffee : uint8_t
{
    espresso,
    cappuccino,
    flat_white,
    americano = 10,
    v60,
    chemex
};

namespace Cpp23
{
    template <typename TEnum>
    constexpr auto to_underlying(TEnum e) noexcept
    {
        return static_cast<std::underlying_type_t<TEnum>>(e);
    }
}

TEST_CASE("enum classes")
{
    Coffee coffee = Coffee::espresso;

    SECTION("implicit conversion from/to integer is not allowed")
    {
        coffee = static_cast<Coffee>(4);
        REQUIRE(static_cast<int>(coffee) == 4);

        REQUIRE(Cpp23::to_underlying(coffee) == 4);
    }
}

enum class DaysOfWeek
{
    Monday,
    Tuesday,
    Wednesday,
    Thursday,
    Friday,
    Saturday,
    Sunday
};

enum OldSchoolDays
{
    Monday,
    Tuesday,
    Wednesday,
    Thursday,
    Friday,
    Saturday,
    Sunday
};

TEST_CASE("enums & containers")
{
    std::vector<std::string> vec = {"Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday", "Sunday"};

    DaysOfWeek day = DaysOfWeek::Monday;
    DaysOfWeek other_day{5};

    std::cout << vec[static_cast<size_t>(DaysOfWeek::Thursday)] << "\n";
    std::cout << vec[Monday] << "\n";

    std::map<DaysOfWeek, std::string> days = {
        {DaysOfWeek::Monday, "Poniedziałek"},
        {DaysOfWeek::Tuesday, "Wtorek"},
        {DaysOfWeek::Wednesday, "Środa"},
        {DaysOfWeek::Thursday, "Czwartek"},
        {DaysOfWeek::Friday, "Piątek"},
        {DaysOfWeek::Saturday, "Sobota"},
        {DaysOfWeek::Sunday, "Niedziela"}};

    std::cout << days[DaysOfWeek::Monday] << "\n";
}

namespace Explain
{
    enum class byte : uint8_t
    {
    };

    byte operator&(byte lhs, byte rhs)
    {
        return static_cast<byte>(static_cast<uint8_t>(lhs) & static_cast<uint8_t>(rhs));
    }

    //...
}

TEST_CASE("std::byte - since C++17")
{
    std::byte byte_value_1{0x12};
    std::byte byte_value_2{0x34};

    std::byte byte_result = byte_value_1 & byte_value_2;

    // char int_result = byte_value_1 + byte_value_2; // integer arithmetic is not allowed for std::byte

    std::cout << "byte value as int: " << std::to_integer<int>(byte_result) << "\n";

    std::vector<std::byte> vec = {std::byte{0x12}, std::byte{0x34}, std::byte{0x56}, std::byte{0x78}};
}

void print(float const x, std::span<const std::byte> const bytes)
{
    std::cout << std::setprecision(6) << std::setw(8) << x << " = { "
              << std::hex << std::uppercase << std::setfill('0');
    for (auto const b : bytes)
        std::cout << std::setw(2) << std::to_integer<int>(b) << ' ';
    std::cout << std::dec << "}\n";
}

TEST_CASE("memory as bytes")
{
    /* mutable */ float data[1]{3.141592f};

    auto const const_bytes = std::as_bytes(std::span{data});

    print(data[0], const_bytes);

    auto const writable_bytes = std::as_writable_bytes(std::span{data});

    // Change the sign bit that is the MSB (IEEE 754 Floating-Point Standard).
    writable_bytes[3] |= std::byte{0B1000'0000};

    print(data[0], const_bytes);
}