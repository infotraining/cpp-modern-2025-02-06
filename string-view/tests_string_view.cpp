#include <algorithm>
#include <array>
#include <catch2/catch_approx.hpp>
#include <catch2/catch_test_macros.hpp>
#include <iostream>
#include <numeric>
#include <string>
#include <string_view>
#include <vector>

using namespace std;
using namespace std::literals;

template <typename Container>
void print_all(const Container& container, std::string_view prefix)
{
    cout << prefix << ": [ ";
    for (const auto& item : container)
        cout << item << " ";
    cout << "]\n";
}

TEST_CASE("string_view")
{
    const char* ctxt = "abc def,ghi";
    
    std::string_view all_ctxt = ctxt;
    REQUIRE(all_ctxt == "abc def,ghi");

    std::string_view token{ctxt + 4, 3};
    REQUIRE(token == "def"sv);

    std::string str = "string text";
    std::string_view all_str = str;
    REQUIRE(all_str == "string text");

    std::string_view sv_empty;
    REQUIRE(sv_empty.data() == nullptr);
    REQUIRE(sv_empty.size() == 0);

    char txt[3] = { 't', 'x', 't' };
    std::string_view sv_txt{txt, 3};
    REQUIRE(sv_txt == "txt");

    print_all(sv_txt, "sv_txt");
}

std::string_view get_token(std::string_view text)
{
    auto pos = text.find_first_of(" ");
    if (pos != std::string_view::npos)
        return std::string_view{text.data(), pos};
    return text;
}

TEST_CASE("get_token")
{
    auto token = get_token("abc def"s); // UB
    REQUIRE(token == "abc");
}