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

std::vector<int> Z2P(const std::vector<int> &z) {
    int len = static_cast<int>(z.size());
    std::vector<int> p(len);

    for (int i = 1; i < len; ++i) {
        for (int j = z[i]; j > 0 and !p[i + j - 1]; --j) {
            p[i + j - 1] = j;
        }
    }
    return p;
}

int main() {
    SetUp();
    PrintVec(Z2P(ReadVec()));
}