#include "utils.hpp"

#include <algorithm>
#include <catch2/catch_test_macros.hpp>
#include <iostream>
#include <map>
#include <memory>
#include <numeric>
#include <string>
#include <vector>
#include <thread>

// https://isocpp.github.io/CppCoreGuidelines/CppCoreGuidelines#S-resource

using namespace std;
using namespace Utils;

namespace LegacyCode
{
    // forward declarations
    Gadget* get_gadget(const std::string& name);
    void use(Gadget* g);
    void use_gadget(Gadget* g);

    // definitions
    Gadget* get_gadget(const std::string& name)
    {
        static int id = 665;
        return new Gadget(++id, name);
    }

    void use(Gadget* g)
    {
        if (g)
            std::cout << "Using " << g->name() << "\n";

        delete g;
    }

    void use_gadget(Gadget* g)
    {
        if (g)
            std::cout << "Using " << g->name() << "\n";
    }
}

TEST_CASE("Legacy hell with dynamic memory")
{
    using namespace LegacyCode;

    // {
    //     Gadget* g = get_gadget("ipad");

    //     use_gadget(g);
    // } // memory leak

    // {
    //     use_gadget(get_gadget("ipad"));
    // } // memory leak

    // {
    //     Gadget* g = get_gadget("ipad");

    //     use(g);

    //     std::cout << g->name() << std::endl; // UB - use after delete
    // }
}

namespace ModernCpp
{
    std::unique_ptr<Gadget> get_gadget(const std::string& name);
    void use(std::unique_ptr<Gadget> g);
    void use_gadget(Gadget* g);

    // definitions
    std::unique_ptr<Gadget> get_gadget(const std::string& name)
    {
        static int id = 665;
        return std::make_unique<Gadget>(++id, name);
    }

    void use(std::unique_ptr<Gadget> g)
    {
        if (g)
            std::cout << "Using " << g->name() << "\n";
    }

    void use_gadget(Gadget* g)
    {
        if (g)
            std::cout << "Using " << g->name() << "\n";
    }
}

void may_throw()
{
    // throw std::runtime_error("ERROR");
}

TEST_CASE("Safer code with smart pointers")
{
    using namespace ModernCpp;

    {
        std::unique_ptr<Gadget> g = get_gadget("ipad");

        use_gadget(g.get());

        LegacyCode::use(g.release());
    }

    {
        use_gadget(get_gadget("ipad").get());
    }

    {
        std::unique_ptr<Gadget> g = get_gadget("ipad");

        use(std::move(g));
    }

    {
        {
            FILE* my_file = fopen("data1.txt", "aw+");

            if (!my_file)
            {
                std::cout << "File not opened!!!\n";
            }
            else
            {
                fprintf(my_file, "abc");
                // may_throw();

                fclose(my_file);
            }
        }

        {
            std::unique_ptr<FILE, int(*)(FILE*)> my_safe_file{fopen("data2.txt", "aw+"), &fclose};

            fprintf(my_safe_file.get(), "abc");
            may_throw();
        }

        {
            auto file_closer = [](FILE* f) {
                std::cout << "Log: file is closed\n";
                fclose(f);
            };

            std::unique_ptr<FILE, decltype(file_closer)> my_safe_file{fopen("data3.txt", "aw+"), file_closer};

            fprintf(my_safe_file.get(), "abc");
            may_throw();
        }
    }
}

TEST_CASE("shared_ptrs & threads")
{
    using namespace std::literals;

    using Dictionary = std::map<int, std::string>;

    std::thread thd_1;
    std::thread thd_2;

    {
        auto dictionary = std::make_shared<Dictionary>();

        dictionary->emplace(1, "one");
        dictionary->emplace(2, "two");

        thd_1 = std::thread([dictionary]() { 
            std::cout << "Thread#1 started..." << std::endl;
            std::this_thread::sleep_for(1s);

            dictionary->emplace(3, "three"); // RC

            auto pos = dictionary->find(1);
            if (pos != dictionary->end())
            {
                std::cout << "item: " << pos->second << "\n";
            }
        });

        thd_2 = std::thread([dictionary]() { 
            std::cout << "Thread#2 started..." << std::endl;
            std::this_thread::sleep_for(2s);

            auto pos = dictionary->find(2);
            if (pos != dictionary->end())
            {
                std::cout << "item: " << pos->second << "\n";
            }
        });
    }

    thd_1.join();
    thd_2.join();
}

