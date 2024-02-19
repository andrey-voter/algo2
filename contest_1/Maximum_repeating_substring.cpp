#include <iostream>
#include <vector>
#include <string>

void SetUp() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);
    std::cout.tie(nullptr);
}

std::vector<int> ZFunc(const std::string &str) {
    int len = static_cast<int>(str.length());
    std::vector<int> zed(len);
    zed[0] = len;
    int left = 0;
    int right = 0;

    for (int index = 1; index < len; ++index) {
        if (index <= right) {
            zed[index] = std::min(right - index + 1, zed[index - left]);
        }
        while (index + zed[index] < len and str[index + zed[index]] == str[zed[index]]) {
            ++zed[index];
        }
        if (index + zed[index] - 1 > right) {
            right = index + zed[index] - 1;
            left = index;
        }
    }
    return zed;
}

int FindMaximumRepeatingSubstring(const std::string &str) {
    int ans = 1;
    int len = static_cast<int>(str.length());
    for (int offset = 0; offset < len; ++offset) {
        auto zed = ZFunc(str.substr(offset));
        for (int i = 1; i < len - offset; ++i) {
            if (zed[i] % i == 0) {
                ans = std::max(ans, zed[i] / i + 1);
            }
        }
    }
    return ans;
}

int main() {
    SetUp();
    std::string str;
    std::getline(std::cin, str);
    std::cout << FindMaximumRepeatingSubstring(str) << std::endl;
}
