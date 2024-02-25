#include <iostream>
#include <vector>

void SetUp() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);
    std::cout.tie(nullptr);
}

void PrintVec(const std::vector<int> &vec) {
    for (int el: vec) {
        std::cout << el << " ";
    }
    std::cout << std::endl;
}

std::vector<int> ReadVec() {
    int len;
    std::cin >> len;
    std::vector<int> result(len);

    for (int i = 0; i < len; ++i) {
        std::cin >> result[i];
    }

    return result;
}

std::vector<int> ZFunc(const std::vector<int> &str) {
    int len = static_cast<int>(str.size());
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

std::vector<int> StringFromP(const std::vector<int> &pi) {
    int len = static_cast<int>(pi.size());
    std::vector<int> result;
    result.reserve(len);

    for (int i = 0; i < len; ++i) {
        if (pi[i]) {
            result.push_back(result[pi[i] - 1]);
        } else {
            result.push_back(i);
        }
    }
    return result;
}

std::vector<int> P2Z(const std::vector<int> &p) {
    return (ZFunc(StringFromP(p)));
}

int main() {
    SetUp();
    PrintVec(P2Z(ReadVec()));
}