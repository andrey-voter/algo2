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

int FindMaximumPeriod(const std::string &str) {
    auto zed = ZFunc(str);
    int ans = 1;
    int len = static_cast<int>(zed.size());
    for (int i = 1; i < len; ++i) {
        if (zed[i] == len - i and len % i == 0) {
            ans = len / i;
            break;
        }
    }
    return ans;
}

int FindMaximumRepeatingSubstring(const std::string &str) {
    int ans = 1;
    int len = static_cast<int>(str.length());
    for (int begin = 0; begin < len; ++begin) {
        for (int cur_len = 1; cur_len < len - begin; ++cur_len) {
            int cur_val = FindMaximumPeriod(str.substr(begin, cur_len));
            if (cur_val > ans) {
                ans = cur_val;
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
