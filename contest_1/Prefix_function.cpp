#include <iostream>
#include <vector>
#include <string>

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

std::vector<int> PrefixFunc(const std::string &str) {
    int len = static_cast<int>(str.length());
    std::vector<int> pi(len);

    for (int index = 1; index < len; ++index) {
        int ii = pi[index - 1];
        while (ii > 0 and str[index] != str[ii]) {
            ii = pi[ii - 1];
        }
        if (str[index] == str[ii]) {
            ++ii;
        }
        pi[index] = ii;
    }
    return pi;
}


int main() {
    SetUp();
    std::string str;
    std::getline(std::cin, str);
    PrintVec(PrefixFunc(str));
}
