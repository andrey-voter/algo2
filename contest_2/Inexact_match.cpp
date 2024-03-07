#include <iostream>
#include <vector>
#include <string>
#include <algorithm>

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


void PrintVec(const std::vector<int> &vec) {
    for (int el: vec) {
        std::cout << el << " ";
    }
    std::cout << std::endl;
}

std::vector<int> Solve(std::string &pattern, std::string &text) {
    if (pattern.size() > text.size()) {
        return std::vector<int>();
    }

    std::vector<int> res;

    auto zed1 = ZFunc(pattern + "#" + text);
    std::reverse(pattern.begin(), pattern.end());
    std::reverse(text.begin(), text.end());
    auto zed2 = ZFunc(pattern + "#" + text);

    int len = static_cast<int>(zed1.size());
    int pattern_size = static_cast<int>(pattern.size());
    int text_size = static_cast<int>(text.size());
    for (int i = pattern_size + 1; i < len - pattern_size + 1; ++i) {
        if (zed1[i] + zed2[text_size + pattern_size - i + 2] >= pattern_size - 1) {
            res.push_back(i - pattern_size);
        }
    }

    return res;
}


int main() {
    SetUp();
    std::string pattern;
    std::cin >> pattern;
    std::string text;
    std::cin >> text;
    auto ans = Solve(pattern, text);
    std::cout << ans.size() << '\n';
    PrintVec(ans);
}