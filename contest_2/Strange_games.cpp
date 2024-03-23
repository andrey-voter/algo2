#include <iostream>
#include <vector>
#include <string>

void SetUp() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);
    std::cout.tie(nullptr);
}

bool Ask(int idx1, int idx2, int nn, int &num_questions) {
    if (idx1 == nn or idx2 == nn) {
        return false;
    }
    --num_questions;
    char sym1, sym2;
    if (idx1 < nn) {
        sym1 = 's';
        idx1 += 1;
    } else {
        sym1 = 't';
        idx1 -= nn;
    }
    if (idx2 < nn) {
        sym2 = 's';
        idx2 += 1;
    } else {
        sym2 = 't';
        idx2 -= nn;
    }
    std::cout << sym1 << ' ' << idx1 << ' ' << sym2 << ' ' << idx2 << std::endl;
    std::string answer;
    std::cin >> answer;
    if (answer == "Yes") {
        return true;
    }
    return false;
}

bool CountStep(int &ii, int &index, int &num_questions, int &cnt, int nn, std::vector<int> &pi) {
    while (num_questions and ii > 0 and !Ask(index, ii, nn, num_questions)) {
        ii = pi[ii - 1];
    }
    if (num_questions) {
        if (Ask(index, ii, nn, num_questions)) {
            ++ii;
        }
        pi[index] = ii;
        if (ii == nn) {
            ++cnt;
        }
        return true;
    }
    return false;
}


int main() {
    SetUp();
    int nn, mm;
    std::cin >> nn >> mm;
    int cnt = 0;
    std::vector<int> pi(nn + mm + 1);
    int round_number = 0;
    int num_questions = 5;
    for (int index = 1; index < nn + mm + 1; ++index) {
        // ask questions
        if (index > nn + round_number + 1) {
            std::cout << "$ " << cnt << std::endl;
            ++round_number;
            if (round_number == mm) {
                return 0;
            }
            num_questions = 5;
        }
        int ii = pi[index - 1];
        bool step_res = CountStep(ii, index, num_questions, cnt, nn, pi);
        while (not step_res) {
            std::cout << "$ " << cnt << std::endl;
            ++round_number;
            if (round_number == mm) {
                return 0;
            }
            num_questions = 5;
            step_res = CountStep(ii, index, num_questions, cnt, nn, pi);
        }
        // answer in the end of each round
        if (not num_questions) {
            std::cout << "$ " << cnt << std::endl;
            ++round_number;
            if (round_number == mm) {
                return 0;
            }
            num_questions = 5;
        }
        if (round_number == mm) {
            std::cout << "$ " << cnt << std::endl;
            return 0;
        }
    }
    std::cout << "$ " << cnt << std::endl;
}
