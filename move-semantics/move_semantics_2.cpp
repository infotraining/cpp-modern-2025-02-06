#include <catch2/catch_test_macros.hpp>
#include "gadget.hpp"
#include <memory>
#include <string>

template <typename T>
class UniquePtr
{
    T* ptr_;

public:
    UniquePtr(nullptr_t)
        : ptr_{nullptr}
    {
    }

    UniquePtr() : ptr_{nullptr}
    {}

    explicit UniquePtr(T* ptr)
        : ptr_{ptr}
    {
    }

    UniquePtr(const UniquePtr&) = delete;
    UniquePtr& operator=(const UniquePtr&) = delete;

    // move constructor
    UniquePtr(UniquePtr&& source) : ptr_{source.ptr_}
    {
        source.ptr_ = nullptr;
    }

    // move assignment operator
    UniquePtr& operator=(UniquePtr&& source)
    {
        if (this != &source)
        {
            delete ptr_;
            ptr_ = source.ptr_;
            source.ptr_ = nullptr;
        }

        return *this;
    }

    ~UniquePtr()
    {
        delete ptr_;
    }

    explicit operator bool() const
    {
        return ptr_ != nullptr;
    }

    T* get() const
    {
        return ptr_;
    }

    T* operator->() const
    {
        return ptr_;
    }

    T& operator*() const
    {
        return ptr_;
    }
};

// TEST_CASE("2---")
// {
//     std::cout << "\n--------------------------\n\n";
// }

TEST_CASE("move semantics - UniquePtr")
{
    UniquePtr<Gadget> pg1{new Gadget{1, "ipad"}};
    
    if (pg1)
        pg1->use();

    UniquePtr<Gadget> pg2 = std::move(pg1); // move constructor called from UniquePtr
    pg2->use();

    pg2 = UniquePtr<Gadget>{new Gadget{2, "smartwatch"}}; 
    pg2 = std::move(pg2); // self assignment

    pg2->use();
}