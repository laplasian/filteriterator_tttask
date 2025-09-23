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

    using namespace std;
    class Solution {
    public:
        string gcdOfStrings(string str1, string str2) {
            string& largest_ = str1.size() > str2.size()? str1 : str2;
            string& smallest_ = str1.size() > str2.size()? str2 : str1;

            vector<int> results{};

            bool success = true;
            int current_divisor = 1;
            while (current_divisor <= smallest_.size()) {
                success = true;
                for (int i = 0; i + current_divisor <= smallest_.size(); i+=current_divisor) {
                    if (!std::equal(smallest_.begin(), smallest_.begin() + current_divisor, smallest_.begin() + i)) {
                        success = false;
                        break;
                    }
                }
                for (int i = 0; i + current_divisor <= largest_.size(); i+=current_divisor) {
                    if (!std::equal(smallest_.begin(), smallest_.begin() + current_divisor, largest_.begin() + i)) {
                        success = false;
                        break;
                    }
                }
                if (success) results.push_back(current_divisor);
                current_divisor++;
                while (largest_.size() % current_divisor != 0 || smallest_.size() % current_divisor != 0) {
                    if (current_divisor >= smallest_.size() + 1) break;
                    current_divisor+=1;
                }
            }
            if (!results.empty()) return {smallest_.begin(), smallest_.begin() + results.back()};
            return "";
        }
    };

    Solution a{};
    std::cout << a.gcdOfStrings(string("ABABAB"), string("ABAB"));

    return 0;
}