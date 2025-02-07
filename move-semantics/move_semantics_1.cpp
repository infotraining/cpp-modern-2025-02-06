#include <catch2/catch_test_macros.hpp>
#include <deque>
#include <iostream>
#include <map>
#include <string>
#include <vector>

#include "gadget.hpp"


std::string full_name(const std::string& first_name, const std::string& last_name)
{
    return first_name + " " + last_name;
}

TEST_CASE("reference binding")
{
    std::string name = "jan";

    SECTION("C++98")
    {
        std::string& ref_name = name;

        const std::string& cref_full_name = full_name(name, "Kowalski");
        REQUIRE(cref_full_name == "jan Kowalski");

        // cref_full_name[0] = 'J'; // ERROR
    }

    SECTION("C++11")
    {
        std::string&& rvalue_ref_full_name = full_name(name, "Kowalski");
        rvalue_ref_full_name[0] = 'J';
        REQUIRE(rvalue_ref_full_name == "Jan Kowalski");

        // std::string&& rvalue_ref_name = name; // ERROR 
    }
}

namespace Explain
{
    class vector
    {
    public:
        void push_back(const std::string& str)
        {
            std::cout << "vector::push_back(const std::string& str) - makes copy of " << str << "\n";
        }

        void push_back(std::string&& str)
        {
            std::cout << "vector::push_back(std::string&& str) - transfers state of " << str << "\n";
        }
    };
}

TEST_CASE("moving objects")
{
    Explain::vector vec;

    std::string txt = "abc";
    vec.push_back(txt); // copy txt into vector
    vec.push_back(std::string{"def"});  // move temporary to vector
    vec.push_back(full_name("Adam", "Nowak")); // move temporary to vector

    std::string backup = txt; // copy by copy constructor
    std::string target = std::move(txt); // moved to target by move constructor
    REQUIRE(target == "abc");
    REQUIRE(txt.empty());
}


std::vector<std::string> create_and_fill()
{
    std::vector<std::string> vec;
 
    std::string str = "text";
 
    vec.push_back(str);
 
    vec.push_back(str + str); 
 
    vec.push_back("text"); // push_back(const std::string&)

    {
        std::string anonymous_object("text");
        vec.push_back(anonymous_object); // push_back(const std::string&)
    }
 
#if __cpp_plus_plus >= 201103L
    vec.push_back(std::move(str));  
#else
    vec.push_back(str);
#endif
 
    return vec;
}

TEST_CASE("create_and_fill")
{
    std::vector<std::string> words = create_and_fill();

    std::vector<std::string> vec;
    vec.push_back("abc");
    vec.push_back("def");
}