#include <iostream>
#include <vector>
#include <string>

void PrintVec(const std::vector<int> &vec) {
    for (int el: vec) {
        std::cout << el << " ";
    }
    std::cout << std::endl;
}

std::vector<int> FuzzyZFunc(const std::string &str, int from_pos = 0) {
    int len = static_cast<int>(str.length()) - from_pos;
    std::vector<int> zed(len);
    int left = 0;
    int right = 0;

    for (int index = 0; index < len; ++index) {
        if (index <= right) {
            zed[index] = std::min(right - index + 1, zed[index - left]);
        }
        while (index + zed[index] < len and (str[zed[index]] == '?' or str[from_pos + index + zed[index]] == str[zed[index]])) {
            ++zed[index];
        }
        if (index + zed[index] - 1 > right) {
            right = index + zed[index] - 1;
            left = index;
        }
    }
    return zed;
}

int main() {
    std::string str = "ab?#ababcabc";
    PrintVec(FuzzyZFunc(str, 4));
}
