#include "gadget.hpp"

#include <catch2/catch_test_macros.hpp>
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

    UniquePtr()
        : ptr_{nullptr}
    {
    }

    explicit UniquePtr(T* ptr)
        : ptr_{ptr}
    {
    }

    UniquePtr(const UniquePtr&) = delete;
    UniquePtr& operator=(const UniquePtr&) = delete;

    // move constructor
    UniquePtr(UniquePtr&& source)
        : ptr_{source.ptr_}
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

    T* release(T* ptr = nullptr)
    {
        T* temp_ptr = ptr_;
        ptr_ = ptr;
        return temp_ptr;
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

UniquePtr<Gadget> create_gadget()
{
    static uint32_t id = 0;

    uint32_t current_id = ++id;
    UniquePtr<Gadget> ptr_g{new Gadget{static_cast<int>(current_id), "Gadget#" + std::to_string(current_id)}};
    return ptr_g;
}

namespace LegacyCode
{
    void use(Gadget* ptr_g);
    void use_and_destroy(Gadget* ptr_g);

    void use(Gadget* ptr_g)
    {
        if (ptr_g)
            ptr_g->use();
    }

    void use_and_destroy(Gadget* ptr_g)
    {
        if (ptr_g)
            ptr_g->use();

        delete ptr_g;
    }
}

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

    UniquePtr<Gadget> pg3 = create_gadget();
    pg3->use();

    {
        std::vector<UniquePtr<Gadget>> gadgets;

        gadgets.push_back(std::move(pg2));
        gadgets.push_back(std::move(pg3));
        gadgets.push_back(create_gadget());
        gadgets.push_back(create_gadget());
        gadgets.push_back(create_gadget());

        for (const auto& ptr_g : gadgets)
            if (ptr_g)
                ptr_g->use();

        LegacyCode::use_and_destroy(gadgets[0].release());
    }
}

struct Matrix
{
    std::vector<std::vector<double>> matrix;

    explicit Matrix(size_t size)
        : matrix(size, std::vector<double>(size))
    {
    }
};

Matrix create_matrix()
{
    Matrix m(1'000'000);

    //..

    return m;
}

TEST_CASE("C++11 Efficiency")
{
    Matrix m = create_matrix(); // move semantics in the worst case
}
