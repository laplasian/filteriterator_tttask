#include <iostream>
#include <vector>
#include <algorithm>
#include <functional>
#include <gtest/gtest.h>
#include <random>
#include <array>
#include <deque>
#include <list>

#if defined(USE_CONCEPTS)
#include "filteriterator.hpp"
#else
#include "filteriterator_SFINAE.hpp"
#endif

struct CustomStruct {
    int id;
    std::string data;

    bool operator==(const CustomStruct& other) const {
        return id == other.id && data == other.data;
    }
};

#define MAKE_RA_ITERS(T_) std::vector<T_>, std::deque<T_>

template <typename T>
class FilterIteratorTypedTest : public ::testing::Test {};
using MyTypes = ::testing::Types<MAKE_RA_ITERS(char), MAKE_RA_ITERS(unsigned char), MAKE_RA_ITERS(short), \
    MAKE_RA_ITERS(unsigned short), MAKE_RA_ITERS(int), MAKE_RA_ITERS(long), MAKE_RA_ITERS(float), MAKE_RA_ITERS(double)>;

TYPED_TEST_CASE(FilterIteratorTypedTest, MyTypes);

/// OPERATOR TESTs
/// cstyle

TYPED_TEST(FilterIteratorTypedTest, FiltersValues) {
    using paramtype = typename TypeParam::value_type;

    TypeParam data;
    TypeParam expected;

    if constexpr (std::is_same_v<paramtype, char> || std::is_same_v<paramtype, unsigned char>) {
        data = {6, 9, 0, 1, 2, 3};
        expected = {6, 9, 3};
    } else if constexpr (std::is_same_v<paramtype, short> || std::is_same_v<paramtype, unsigned short>) {
        data = {6, 9, 0, 1, 2, 3};
        expected = {6, 9, 3};
    } else if constexpr (std::is_same_v<paramtype, int> || std::is_same_v<paramtype, unsigned int>) {
        data = {6, 9, 0, 1, 2, 3};
        expected = {6, 9, 3};
    } else if constexpr (std::is_same_v<paramtype, long>) {
        data = {6, 9, 0, 1, 2, 3};
        expected = {6, 9, 3};
    } else if constexpr (std::is_same_v<paramtype, float>) {
        data = {6.1f, 9.2f, 0.3f, 1.4f, 2.0f, 3.5f};
        expected = {6.1f, 9.2f, 3.5f};
    } else if constexpr (std::is_same_v<paramtype, double>) {
        data = {6.1, 9.2, 0.3, 1.4, 2.0, 3.5};
        expected = {6.1, 9.2, 3.5};
    }

    auto range = iterator::filter_range(data.begin(), data.end(), [](paramtype val){ return val > 2; });

    TypeParam result;
    std::copy(range.begin(), range.end(), std::back_inserter(result));

    EXPECT_EQ(result.size(), expected.size());
    auto res_it = result.begin();
    auto exp_it = expected.begin();
    for (; res_it != result.end(); ++res_it, ++exp_it) {
        if constexpr (std::is_floating_point_v<paramtype>) {
            EXPECT_NEAR(*res_it, *exp_it, 10e-6);
        } else {
            EXPECT_EQ(*res_it, *exp_it);
        }
    }
}

TYPED_TEST(FilterIteratorTypedTest, EmptyRange) {
    TypeParam data = {};
    using paramtype = typename TypeParam::value_type;
    auto range = iterator::filter_range(data.begin(), data.end(), [](paramtype v){ return v > 0; });
    EXPECT_EQ(range.begin(), range.end());
    EXPECT_EQ(std::distance(range.begin(), range.end()), 0);
}

TYPED_TEST(FilterIteratorTypedTest, NoMatchingElements) {
    using paramtype = typename TypeParam::value_type;
    TypeParam data;

    if constexpr (std::is_same_v<paramtype, char> || std::is_same_v<paramtype, unsigned char>) {
        data = {1, 2, 3, 4, 5};
    } else if constexpr (std::is_same_v<paramtype, short> || std::is_same_v<paramtype, unsigned short>) {
        data = {1, 2, 3, 4, 5};
    } else if constexpr (std::is_same_v<paramtype, int> || std::is_same_v<paramtype, unsigned int>) {
        data = {1, 2, 3, 4, 5};
    } else if constexpr (std::is_same_v<paramtype, long>) {
        data = {1, 2, 3, 4, 5};
    } else if constexpr (std::is_same_v<paramtype, float>) {
        data = {1.1f, 2.2f, 3.3f, 4.4f, 5.5f};
    } else if constexpr (std::is_same_v<paramtype, double>) {
        data = {1.1, 2.2, 3.3, 4.4, 5.5};
    }

    auto range = iterator::filter_range(data.begin(), data.end(), [](paramtype v){ return v > 10; });
    EXPECT_EQ(range.begin(), range.end());
}

TYPED_TEST(FilterIteratorTypedTest, AllElementsMatch) {
    using paramtype = typename TypeParam::value_type;
    TypeParam data;

    if constexpr (std::is_same_v<paramtype, char> || std::is_same_v<paramtype, unsigned char>) {
        data = {10, 20, 30};
    } else if constexpr (std::is_same_v<paramtype, short> || std::is_same_v<paramtype, unsigned short>) {
        data = {10, 20, 30};
    } else if constexpr (std::is_same_v<paramtype, int> || std::is_same_v<paramtype, unsigned int>) {
        data = {10, 20, 30};
    } else if constexpr (std::is_same_v<paramtype, long>) {
        data = {10, 20, 30};
    } else if constexpr (std::is_same_v<paramtype, float>) {
        data = {10.1f, 20.2f, 30.3f};
    } else if constexpr (std::is_same_v<paramtype, double>) {
        data = {10.1, 20.2, 30.3};
    }

    auto range = iterator::filter_range(data.begin(), data.end(), [](paramtype v){ return v > 5; });

    TypeParam result;
    std::copy(range.begin(), range.end(), std::back_inserter(result));

    EXPECT_EQ(result.size(), data.size());
    auto res_it = result.begin();
    auto data_it = data.begin();
    for (; res_it != result.end(); ++res_it, ++data_it) {
        if constexpr (std::is_floating_point_v<paramtype>) {
            EXPECT_NEAR(*res_it, *data_it, 10e-6);
        } else {
            EXPECT_EQ(*res_it, *data_it);
        }
    }
}

TYPED_TEST(FilterIteratorTypedTest, FirstAndLastElementsMatch) {
    using paramtype = typename TypeParam::value_type;
    TypeParam data = {static_cast<paramtype>(10), static_cast<paramtype>(1), static_cast<paramtype>(2),
        static_cast<paramtype>(3), static_cast<paramtype>(20)};
    TypeParam expected = {10, 20};
    auto range = iterator::filter_range(data.begin(), data.end(), [](paramtype v){ return static_cast<int>(v) % 10 == 0; });

    TypeParam result;
    std::copy(range.begin(), range.end(), std::back_inserter(result));
    EXPECT_EQ(result.size(), expected.size());
    EXPECT_EQ(result, expected);
}

TEST(FilterIteratorTypedTest, RandomData) {
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
    EXPECT_EQ(result, expected);
}

TYPED_TEST(FilterIteratorTypedTest, CStyle) {
    using paramtype = typename TypeParam::value_type;
    paramtype arr[6] = {};
    std::vector<paramtype> expected;

    if constexpr (std::is_same_v<paramtype, char> || std::is_same_v<paramtype, unsigned char>) {
        paramtype arr_[6] = {6, 9, 0, 1, 2, 3};
        memcpy(arr, arr_, sizeof(arr));
        expected = {6, 9, 3};
    } else if constexpr (std::is_same_v<paramtype, short> || std::is_same_v<paramtype, unsigned short>) {
        paramtype arr_[6] = {6, 9, 0, 1, 2, 3};
        memcpy(arr, arr_, sizeof(arr));
        expected = {6, 9, 3};
    } else if constexpr (std::is_same_v<paramtype, int> || std::is_same_v<paramtype, unsigned int>) {
        paramtype arr_[6] = {6, 9, 0, 1, 2, 3};
        memcpy(arr, arr_, sizeof(arr));
        expected = {6, 9, 3};
    } else if constexpr (std::is_same_v<paramtype, long>) {
        paramtype arr_[6] = {6, 9, 0, 1, 2, 3};
        memcpy(arr, arr_, sizeof(arr));
        expected = {6, 9, 3};
    } else if constexpr (std::is_same_v<paramtype, float>) {
        paramtype arr_[6] = {6.1f, 9.2f, 0.3f, 1.4f, 2.0f, 3.5f};
        memcpy(arr, arr_, sizeof(arr));
        expected = {6.1f, 9.2f, 3.5f};
    } else if constexpr (std::is_same_v<paramtype, double>) {
        paramtype arr_[6] = {6.1, 9.2, 0.3, 1.4, 2.0, 3.5};
        memcpy(arr, arr_, sizeof(arr));
        expected = {6.1, 9.2, 3.5};
    }

    auto range = iterator::filter_range(std::begin(arr), std::end(arr), [](paramtype val){ return val > 2; });

    std::vector<paramtype> result;
    std::copy(range.begin(), range.end(), std::back_inserter(result));

    EXPECT_EQ(result.size(), expected.size());
    auto res_it = result.begin();
    auto exp_it = std::begin(expected);
    for (; res_it != result.end(); ++res_it, ++exp_it) {
        if constexpr (std::is_floating_point_v<paramtype>) {
            EXPECT_NEAR(*res_it, *exp_it, 10e-6);
        } else {
            EXPECT_EQ(*res_it, *exp_it);
        }
    }
}

TEST(FilterIteratorTypedTest, CustomType) {
    std::vector<CustomStruct> data = {{1, "Kovalenko Pavel"}, {2, "Kvasnikov Lev"},
        {3, "Trifautsan Artem"}, {4, "Shidlovskaia Kristina"}};
    std::vector<CustomStruct> expected = {{1, "Kovalenko Pavel"}, {2, "Kvasnikov Lev"}};

    auto range = iterator::filter_range(data.begin(), data.end(), [](const CustomStruct &s) {
        return s.data.rfind("K", 0) == 0;
    });

    std::vector<CustomStruct> result(range.begin(), range.end());
    EXPECT_EQ(result, expected);
}

TEST(FilterIteratorTypedTest, Operators) {
    std::vector<int> data = {1, 2, 3, 4, 5, 6};
    auto pred = [](int x){ return x % 2 == 0; };
    iterator::filter_iterator it(data.begin(), data.end(), pred);

    std::vector<std::string> data1 = {"lol", "kek"};
    auto pred1 = [](std::string x){ return true; };
    iterator::filter_iterator it1(data1.begin(), data1.end(), pred1);
    EXPECT_EQ(it1->data(), data1[0].data());
    ++it1;
    EXPECT_EQ(it1->data(), data1[1].data());

    // Test operator*
    EXPECT_EQ(*it, 2);

    // Test operator++
    ++it;
    EXPECT_EQ(*it, 4);

    // Test operator++ (postfix)
    it++;
    EXPECT_EQ(*it, 6);

    // Test operator== and operator!=
    iterator::filter_iterator it2(data.begin(), data.end(), pred);
    iterator::filter_iterator it3(data.begin(), data.end(), pred);
    EXPECT_TRUE(it3 == it2);
    ++it3;
    EXPECT_TRUE(it3 != it2);

}

class MyComp {
    public:
    bool operator()(int a) {
        count++;
        return a > 2;
    }
    [[nodiscard]] int get() const {
        return count;
    }
private:
    int count {};
};

TEST(FilterIteratorTypedTest, CompareFunctionBehavior) {
    std::vector vec = {1,2,3,4,5,6};
    std::vector expected = {3,4,5,6};
    MyComp Comp{};
    auto range = iterator::filter_range(vec.begin(), vec.end(), std::ref(Comp));
    std::vector result(range.begin(), range.end());
    EXPECT_EQ(Comp.get(), 9);
    Comp(0);
    EXPECT_EQ(Comp.get(), 10);
    EXPECT_EQ(result, expected);
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}