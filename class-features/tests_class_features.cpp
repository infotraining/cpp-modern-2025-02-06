#include <catch2/catch_test_macros.hpp>
#include <iostream>
#include <string>
#include <vector>
#include <memory>

using namespace std;

class NoCopyable
{
protected:
    NoCopyable() = default;

public:
    NoCopyable(const NoCopyable&) = delete;
    NoCopyable& operator=(const NoCopyable&) = delete;
};

class Gadget
{
    inline static uintmax_t id_{};
    
    static uintmax_t gen_id()
    {
        auto current_id = ++id_;
        std::cout << "gen_id(): " << current_id << "\n";
        return current_id;
    }

public:
    uintmax_t id = gen_id();
    std::string name = "(Not Set)";

    Gadget() = default;
    virtual ~Gadget() = default;

    explicit Gadget(uintmax_t id)
        : Gadget{id, "Gadget-"s + std::to_string(id)} // delegating init to other constructor
    {
    }

    explicit Gadget(const std::string& name)
        : name{name}
    {
    }

    Gadget(uintmax_t id, const std::string& name)
        : id{id}
        , name{name}
    {
    }

    virtual void status() const
    {
        std::cout << "Gadget{id: " << static_cast<uintmax_t>(id) << ", name: " << name << "}\n";
    }
};

class HyperGadget : public Gadget
{
    std::string description{"(Not-Set)"};

public:
    using Gadget::Gadget; // inheritance of all base class constructors

    explicit HyperGadget(const std::string& name)
        : Gadget{name}
        , description{"Hyper-"s + name}
    {
    }

    void extra_fun()
    {
        std::cout << "HypeGadget{ " << id << ", " << name << "}::extra_fun()\n";
        std::cout << description << "\n";
    }

    void status() const override
    {
        std::cout << "HyperGadget{id: " << static_cast<uintmax_t>(id) << ", name: " << name << "} - " << description << "\n";
    }
};

TEST_CASE("gen_id() and constructors")
{
    Gadget g42{42, "ipad"};
    g42.status();

    Gadget g1{};
    g1.status();
}

TEST_CASE("overriding virtual functions")
{
    std::unique_ptr<Gadget> ptr_g = std::make_unique<HyperGadget>("ipad");

    ptr_g->status();
}

// TEST_CASE("class features")
// {
//     Gadget g1;
//     g1.status();

//     Gadget g2{"ipad"};
//     g2.status();

//     std::vector<Gadget> gadgets(10);
//     for(const auto& g : gadgets)
//         g.status();

//     Gadget g3{665};
//     g3.status();

//     Gadget g4{101};

//     HyperGadget h1{"hyper ipad"};
//     h1.extra_fun();

//     HyperGadget h2{"smartwatch"};
//     h1 = h2;
//     h1.extra_fun();
// }

void integral_only(int a)
{
    cout << "integral_only: " << a << endl;
}

void integral_only(double d) = delete;

TEST_CASE("delete any function")
{
    integral_only(42);
    // integral_only(42.55); // ERROR
    // integral_only(42.665f); // ERROR
}