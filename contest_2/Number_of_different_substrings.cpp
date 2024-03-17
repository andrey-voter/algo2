#include <iostream>
#include <vector>
#include <string>
#include <map>

void PrintVec(const std::vector<int64_t> &vec) {
    for (auto el: vec) {
        std::cout << el << " ";
    }
    std::cout << std::endl;
}

class SuffixAutomaton {
public:
    SuffixAutomaton(const std::string &str) : last_(0), size_(1) {
        automaton_.reserve(str.size() * 2 + 1);
        automaton_.push_back({0, -1});

        for (char c: str) {
            AddSYm(c);
        }
    }

    void AddSYm(char sym) {
        int64_t cur = size_++;
        automaton_.push_back({automaton_[last_].len + 1});
        int64_t state_p;
        for (state_p = last_; state_p != -1 && !automaton_[state_p].next.count(
                sym); state_p = automaton_[state_p].link) {
            automaton_[state_p].next[sym] = cur;
        }

        if (state_p == -1) {
            automaton_[cur].link = 0;
        } else {
            int64_t state_q = automaton_[state_p].next[sym];
            if (automaton_[state_p].len + 1 == automaton_[state_q].len) {
                automaton_[cur].link = state_q;
            } else {
                int64_t clone = size_++;
                automaton_.push_back({automaton_[state_p].len + 1, automaton_[state_q].link,
                                      automaton_[state_q].next});
                for (; state_p != -1 && automaton_[state_p].next[sym] ==
                                        state_q; state_p = automaton_[state_p].link) {
                    automaton_[state_p].next[sym] = clone;
                }
                automaton_[state_q].link = automaton_[cur].link = clone;
            }
        }
        last_ = cur;
    }

    int64_t CountDistinctSubstrings() {
        int64_t ans = 0;
        for (int64_t i = 1; i < size_; ++i) {
            ans += automaton_[i].len - automaton_[automaton_[i].link].len;
        }
        return ans;
    }

private:
    struct State {
        int64_t len, link;
        std::map<char, int64_t> next;
    };

    int64_t last_;
    int64_t size_;
    std::vector<State> automaton_;

};

void SetUp() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);
    std::cout.tie(nullptr);
}

int64_t CountNumberOfSubstrings(const std::string &str) {
    SuffixAutomaton suffix_automaton = SuffixAutomaton(str);
    return suffix_automaton.CountDistinctSubstrings();
}


int main() {
    SetUp();
    std::string str;
    std::cin >> str;
    std::cout << CountNumberOfSubstrings(str) << std::endl;
}
