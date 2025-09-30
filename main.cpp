#include <iostream>
#include <vector>
#include <array>
#include <complex>
#include <deque>
#include <list>
#include <map>
#include "filteriterator_SFINAE.hpp"


int main() {
    std::vector numbers = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 };

    auto filtered_range = iterator::filter_range(numbers.begin(), numbers.end(), [](int num) { return num % 2 == 0; });
    auto filtered_range2 = iterator::filter_range(numbers.begin(), numbers.end(), [](int num) { return num < 5 && num > 1 ; });
    for (auto it:filtered_range) {
        std::cout << it << std::endl;
    }
    return 0;
}