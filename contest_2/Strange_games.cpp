#include <iostream>
#include <vector>
#include <string>
#include <unordered_map>

void Reset(int &round_number, int &num_questions, int cnt, int m);

void SetUp() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);
    std::cout.tie(nullptr);
}

bool Ask(int idx1, int idx2, int n, int &num_questions, std::unordered_map<std::string, bool> &already_asked,
         int &round_number, int &cnt, int m) {
    if (idx1 == n or idx2 == n) {
        return false;
    }
    std::string sym1, sym2;
    if (idx1 < n) {
        sym1 = 's';
        idx1 += 1;
    } else {
        sym1 = 't';
        idx1 -= n;
    }
    if (idx2 < n) {
        sym2 = 's';
        idx2 += 1;
    } else {
        sym2 = 't';
        idx2 -= n;
    }
    std::string question = sym1 + " " + std::to_string(idx1) + " " + sym2 + " " + std::to_string(idx2);
    auto ans = already_asked.find(question);
    if (ans != already_asked.end()) {
        return ans->second;
    }
    if (not num_questions) {
        Reset(round_number, num_questions, cnt, m);
    }
    --num_questions;
    std::cout << question << std::endl;
    std::string answer;
    std::cin >> answer;
    if (answer == "Yes") {
        already_asked[question] = true;
        return true;
    }
    already_asked[question] = false;
    return false;
}


void Reset(int &round_number, int &num_questions, int cnt, int m) {
    std::cout << "$ " << cnt << std::endl;
    ++round_number;
    if (round_number == m) {
        exit(0);
    }
    num_questions = 5;
}


int main() {
    SetUp();

    int n, m;
    std::cin >> n >> m;
    int cnt = 0;
    std::vector<int> pi(n + m + 1, 0);
    std::unordered_map<std::string, bool> already_asked;
    int round_number = 0;
    int num_questions = 5;

    for (int index = 1; index < n + m + 1; ++index) {
        // ask questions
        int ii = pi[index - 1];
        if (index > n + round_number + 1) {
            Reset(round_number, num_questions, cnt, m);
        }
        while (ii and !Ask(index, ii, n, num_questions, already_asked, round_number, cnt, m)) {
            ii = pi[ii - 1];
        }
        if (Ask(index, ii, n, num_questions, already_asked, round_number, cnt, m)) {
            ++ii;
        }

        pi[index] = ii;
        if (ii == n) {
            ++cnt;
        }
    }
    std::cout << "$ " << cnt << std::endl;
    return 0;
}
