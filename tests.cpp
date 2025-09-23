#include <iostream>
#include <vector>
#include <algorithm>
#include <functional>
#include <gtest/gtest.h>
#include <random>
#include <array>
#include <list>

#if defined(USE_CONCEPTS)
#include "filteriterator_SFINAE.hpp"
#else
#include "filteriterator.hpp"
#endif

struct CustomStruct {
    int id;
    std::string data;

    bool operator==(const CustomStruct& other) const {
        return id == other.id && data == other.data;
    }
};


template <typename T>
class FilterIteratorTypedTest : public ::testing::Test {};

using MyTypes = ::testing::Types<int, unsigned int, long, float, double>;
TYPED_TEST_SUITE(FilterIteratorTypedTest, MyTypes);

TYPED_TEST(FilterIteratorTypedTest, FiltersValues) {
    using T = TypeParam;
    std::vector<T> data = {T(6), T(9), T(0), T(1), T(2), T(3.5)};
    std::vector<T> expected = {T(6), T(9), T(3.5)};

    auto range = iterator::filter_range(data.begin(), data.end(), [](T val){ return val > 2; });

    std::vector<T> result;
    std::copy(range.begin(), range.end(), std::back_inserter(result));

    ASSERT_EQ(result.size(), expected.size());
    for (size_t i = 0; i < result.size(); ++i) {
        if constexpr (std::is_floating_point_v<T>) {
            ASSERT_FLOAT_EQ(result[i], expected[i]);
        } else {
            ASSERT_EQ(result[i], expected[i]);
        }
    }
}

TEST(FilterIteratorTest, EmptyRange) {
    std::vector<int> data = {};
    auto range = iterator::filter_range(data.begin(), data.end(), [](int v){ return v > 0; });
    ASSERT_EQ(range.begin(), range.end());
    ASSERT_EQ(std::distance(range.begin(), range.end()), 0);
}

TEST(FilterIteratorTest, NoMatchingElements) {
    std::vector<int> data = {1, 2, 3, 4, 5};
    auto range = iterator::filter_range(data.begin(), data.end(), [](int v){ return v > 10; });
    ASSERT_EQ(range.begin(), range.end());
}

TEST(FilterIteratorTest, AllElementsMatch) {
    std::vector<int> data = {10, 20, 30};
    auto range = iterator::filter_range(data.begin(), data.end(), [](int v){ return v > 5; });

    std::vector<int> result;
    std::copy(range.begin(), range.end(), std::back_inserter(result));

    ASSERT_EQ(result, data);
}

TEST(FilterIteratorTest, FirstAndLastElementsMatch) {
    std::vector<int> data = {10, 1, 2, 3, 20};
    std::vector<int> expected = {10, 20};
    auto range = iterator::filter_range(data.begin(), data.end(), [](int v){ return v % 10 == 0; });

    std::vector<int> result(range.begin(), range.end());
    ASSERT_EQ(result, expected);
}

TEST(FilterIteratorTest, WorksWithStdList) {
    std::list<int> data = {1, 2, 3, 4, 5, 6};
    std::list<int> expected = {2, 4, 6};
    auto range = iterator::filter_range(data.begin(), data.end(), [](int v){ return v % 2 == 0; });

    std::list<int> result(range.begin(), range.end());
    ASSERT_EQ(result, expected);
}

TEST(FilterIteratorTest, RandomData) {
    std::vector<int> data;
    std::vector<int> expected;
    std::mt19937 gen(42);
    std::uniform_int_distribution<> distrib(1, 1000);

    for (int i = 0; i < 500; ++i) {
        int val = distrib(gen);
        data.push_back(val);
        if (val > 500) {
            expected.push_back(val);
        }
    }

    auto range = iterator::filter_range(data.begin(), data.end(), [](int v){ return v > 500; });
    std::vector<int> result(range.begin(), range.end());
    ASSERT_EQ(result, expected);
}

TEST(FilterIteratorTest, CustomType) {
    std::vector<CustomStruct> data = {{1, "Kovalenko Pavel"}, {2, "Kvasnikov Lev"}, {3, "Trifautsan Artem"}, {4, "Shidlovskaia Kristina"}};
    std::vector<CustomStruct> expected = {{1, "Kovalenko Pavel"}, {2, "Kvasnikov Lev"}};

    auto range = iterator::filter_range(data.begin(), data.end(), [](const CustomStruct &s) {
        return s.data.rfind("K", 0) == 0;
    });
    std::vector<CustomStruct> result(range.begin(), range.end());
    ASSERT_EQ(result, expected);
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}