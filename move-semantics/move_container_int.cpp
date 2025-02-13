#include <catch2/catch_section_info.hpp>
#include <catch2/catch_test_macros.hpp>
#include <iostream>
#include <utility>

class ContainerInt
{
public:
    ContainerInt(size_t size, int value)
        : size_{size}
        , items_{new int[size_]}
    {
        std::fill_n(items_, size_, value);
    }

    ContainerInt(std::initializer_list<int> values)
    {
        size_ = values.size();
        items_ = new int[size_];

        std::copy(values.begin(), values.end(), items_);
    }

    // copy constructor
    ContainerInt(const ContainerInt& source)
        : size_{source.size_}
        , items_{new int[size_]}
    {
        std::copy(source.items_, source.items_ + size_, items_);

        std::cout << "Container(cc: ";
        print_items();
        std::cout << ");\n";
    }

    // copy assignment
    ContainerInt& operator=(const ContainerInt& source)
    {
        if (this != &source)
        {
            delete[] items_;

            size_ = source.size_;
            items_ = new int[size_];

            std::copy(source.items_, source.items_ + size_, items_);

            std::cout << "Container(ca: ";
            print_items();
            std::cout << ");\n";
        }

        return *this;
    }

    // TODO: move constr
    ContainerInt(ContainerInt&& source) noexcept
        : size_{std::exchange(source.size_, 0)}
        , items_{std::exchange(source.items_, nullptr)}
    {
        // std::cout << "Container(mv: ";
        // print_items();
        // std::cout << ");\n";
    }

    // TODO: move assignment
    ContainerInt& operator=(ContainerInt&& source) noexcept
    {
        if (this != &source)
        {
            delete[] items_;

            size_ = std::exchange(source.size_, 0);
            items_ = std::exchange(source.items_, nullptr);

            // std::cout << "Container(ma: ";
            // print_items();
            // std::cout << ");\n";
        }

        return *this;
    }

    ~ContainerInt() noexcept
    {
        delete[] items_;
    }

    int& operator[](size_t index)
    {
        return items_[index];
    }

    const int& operator[](size_t index) const
    {
        return items_[index];
    }

    size_t size() const noexcept
    {
        return size_;
    }

    using iterarator = int*;
    using const_iterator = const int*;

    iterarator begin() noexcept
    {
        return items_;
    }

    iterarator end() noexcept
    {
        return items_ + size_;
    }

    const_iterator begin() const noexcept
    {
        return items_;
    }

    const_iterator end() const noexcept
    {
        return items_ + size_;
    }

    const_iterator cbegin() const noexcept
    {
        return items_;
    }

    const_iterator cend() const noexcept
    {
        return items_ + size_;
    }

private:
    size_t size_;
    int* items_;

    void print_items() const
    {
        for (const auto& item : *this)
        {
            std::cout << item << " ";
        }
    }
};

namespace ModernCpp
{
    class ContainerInt
    {
    public:
        ContainerInt(size_t size, int value)
            : items_(size, value)
        {
        }

        ContainerInt(std::initializer_list<int> values)
            : items_(values)
        {
        }

        int& operator[](size_t index)
        {
            return items_[index];
        }

        const int& operator[](size_t index) const
        {
            return items_[index];
        }

        size_t size() const
        {
            return items_.size();
        }

        using iterarator = std::vector<int>::iterator;
        using const_iterator = std::vector<int>::const_iterator;

        iterarator begin()
        {
            return items_.begin();
        }

        iterarator end()
        {
            return items_.end();
        }

        const_iterator begin() const
        {
            return items_.begin();
        }

        const_iterator end() const
        {
            return items_.end();
        }

        const_iterator cbegin() const
        {
            return items_.cbegin();
        }

        const_iterator cend() const
        {
            return items_.cend();
        }

    private:
        std::vector<int> items_;

        void print_items() const
        {
            for (const auto& item : *this)
            {
                std::cout << item << " ";
            }
        }
    };
}

template <typename T>
void print_all(const T& container, std::string_view prefix)
{
    std::cout << prefix << ": ";
    for (const auto& item : container)
    {
        std::cout << item << " ";
    }
    std::cout << std::endl;
}

TEST_CASE("implement move semantics")
{

    SECTION("move constructor")
    {
        std::cout << "-------------------------\n";

        ContainerInt c1 = {10, 42};
        print_all(c1, "c1");

        ContainerInt c2{std::move(c1)}; // move constructor // ContainerInt{ContainerInt&&}

        std::cout << "After move construction" << std::endl;
        print_all(c1, "c1");
        print_all(c2, "c2");

        REQUIRE(c1.size() == 0);
        REQUIRE(c2.size() == 2);
        REQUIRE(c2[0] == 10);
        REQUIRE(c2[1] == 42);
    }

    SECTION("move assignment")
    {
        std::cout << "-------------------------\n";

        ContainerInt c1 = {10, 42};
        print_all(c1, "c1");

        ContainerInt c2{5, 1, 3};
        print_all(c2, "c2");

        c2 = std::move(c1); // move assignment
        // c2 = static_cast<ContainerInt&&>(c1); // std::move is static_cast to rvalue-ref
        std::cout << "After move assignment" << std::endl;
        print_all(c1, "c1");
        print_all(c2, "c2");

        REQUIRE(c1.size() == 0);
        REQUIRE(c2.size() == 2);
        REQUIRE(c2[0] == 10);
        REQUIRE(c2[1] == 42);

        ContainerInt c3 = std::exchange(c2, ContainerInt{665, 667});
    }
}

TEST_CASE("std::move")
{
    SECTION("for primitive types - makes copy")
    {
        int x = 42;
        int y = std::move(x); // copy of x to y
        REQUIRE(x == y);

        int* ptr_x = &x;
        int* ptr_y = std::move(ptr_x); // copy of ptr_x to ptr_y
        REQUIRE(ptr_x == ptr_y);
    }

    SECTION("for custom/library")
    {
        std::vector<int> vec = {1, 2, 3, 4};
        std::vector<int> target = std::move(vec);
        REQUIRE(vec.size() == 0);
        REQUIRE(target == std::vector{1, 2, 3, 4});

        SECTION("const object is copied")
        {
            const std::vector<int> vec = {1, 2, 3, 4};
            std::vector<int> target = std::move(vec);
            REQUIRE(target == vec);
        }
    }
}

struct MyData
{
    int id{};
    std::string name;
    ContainerInt data = {1, 2, 3};

    inline static std::string class_id = "MyData";

    MyData() = default;

    MyData(int id, std::string name, ContainerInt data)
        : id{id}
        , name{std::move(name)}
        , data{std::move(data)}
    {
    }
};

TEST_CASE("MyData - default special ops")
{
    MyData md1{1, "one", {1, 2, 3}};

    MyData md2 = md1; // cc

    MyData md3 = std::move(md1); // mv
}

TEST_CASE("vector & push_back")
{
    std::vector<ContainerInt> vec;
    vec.push_back(ContainerInt{1});

    for(int i = 2; i <= 16; ++i)
    {
        std::cout << "---------\n";
        vec.push_back(ContainerInt{i});
    }

    std::cout << "---------\n";
    vec.push_back(ContainerInt{17});

    ContainerInt c = {1, 2, 3};

    decltype(c.begin()) it;

    constexpr bool is_size_noexcept = noexcept(c.size()); // noexcept operator
}