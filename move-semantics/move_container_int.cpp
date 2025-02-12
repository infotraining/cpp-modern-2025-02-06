#include <catch2/catch_test_macros.hpp>
#include <catch2/catch_section_info.hpp>
#include <iostream>

class ContainerInt
{
public:
    ContainerInt(size_t size, int value) : size_{size}, items_{new int[size_]}
    {
        std::fill_n(items_, size_, value);
    }

    ContainerInt(std::initializer_list<int> values)
    {
        size_ = values.size();
        items_ = new int[size_];

        std::copy(values.begin(), values.end(), items_);
    }

    ContainerInt(const ContainerInt& source)
        : size_{source.size_}
        , items_{new int[size_]}
    {
        std::copy(source.items_, source.items_ + size_, items_);
    }

    ContainerInt& operator=(const ContainerInt& source)
    {
        if (this != &source)
        {
            delete[] items_;

            size_ = source.size_;
            items_ = new int[size_];

            std::copy(source.items_, source.items_ + size_, items_);
        }

        return *this;
    }

    ~ContainerInt()
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

    size_t size() const
    {
        return size_;
    }

    using iterarator = int*;
    using const_iterator = const int*;

    iterarator begin()
    {
        return items_;
    }

    iterarator end()
    {
        return items_ + size_;
    }

    const_iterator begin() const
    {
        return items_;
    }

    const_iterator end() const
    {
        return items_ + size_;
    }

    const_iterator cbegin() const
    {
        return items_;
    }

    const_iterator cend() const
    {
        return items_ + size_;
    }
private:
    size_t size_;
    int* items_;
};

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

        ContainerInt c2 = std::move(c1);

        std::cout << "After move construction" << std::endl;
        print_all(c1, "c1");
        print_all(c2, "c2");

        // REQUIRE(c1.size() == 0);
        // REQUIRE(c2.size() == 2);
        // REQUIRE(c2[0] == 10);
        // REQUIRE(c2[1] == 42);
    }

    SECTION("move assignment")
    {
        std::cout << "-------------------------\n";

        ContainerInt c1 = {10, 42};
        print_all(c1, "c1");
        
        ContainerInt c2{5, 1, 3};
        print_all(c2, "c2");

        c2 = std::move(c1);

        std::cout << "After move assignment" << std::endl;
        print_all(c1, "c1");
        print_all(c2, "c2");


        // REQUIRE(c1.size() == 0);
        // REQUIRE(c2.size() == 2);
        // REQUIRE(c2[0] == 10);
        // REQUIRE(c2[1] == 42);
    }
}