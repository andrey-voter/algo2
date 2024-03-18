#include <iostream>
#include <vector>
#include <string>
#include <map>

void SetUp() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);
    std::cout.tie(nullptr);
}

class SuffixAutomaton {
public:
    SuffixAutomaton() : last_(0), size_(1) {
        automaton_.reserve(kDefaultSize);
        automaton_.push_back({0, -1});
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

    void AddString(const std::string &str) {
        for (char c: str) {
            AddSYm(c);
        }
    }

    bool Find(const std::string &str) {
        int64_t cur = 0;
        bool break_flag = false;
        for (char c: str) {
            if (automaton_[cur].next.count(c)) {
                cur = automaton_[cur].next[c];
            } else {
                break_flag = true;
                break;
            }
        }
        if (break_flag) {
            return false;
        }
        return true;
    }

private:
    struct State {
        int64_t len, link;
        std::map<char, int64_t> next;
    };

    int64_t last_;
    int64_t size_;
    std::vector<State> automaton_;
    static constexpr int64_t kDefaultSize = 100'000;
};

std::string PrepareString(const std::string &input) {
    std::string res = input.substr(2);
    for (char &c: res) {
        c = std::tolower(c);
    }
    return res;
}

int main() {
    SetUp();
    std::string input;
    SuffixAutomaton automaton;

    while (std::getline(std::cin, input)) {
        std::string prepared = PrepareString(input);
        if (input[0] == '?') {
            if (automaton.Find(prepared)) {
                std::cout << "YES\n";
            } else {
                std::cout << "NO\n";
            }
        } else {
            automaton.AddString(prepared);
        }
    }
    return 0;
}
