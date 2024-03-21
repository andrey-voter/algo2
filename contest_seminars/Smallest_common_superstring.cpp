#include <iostream>
#include <vector>
#include <string>
#include <algorithm>


void SetUp() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);
    std::cout.tie(nullptr);
}

std::vector<std::string> ReadStrings(int cnt) {
    std::vector<std::string> res;
    while (cnt--) {
        std::string current_string;
        std::cin >> current_string;
        res.push_back(current_string);
    }
    return res;
}

void PrintStrings(const std::vector<std::string> &strings) {
    for (auto vec: strings) {
        std::cout << vec << std::endl;;
    }
}

class MultiDimensionalCorasick {
public:
    MultiDimensionalCorasick(int num_strings) : size_(1), num_strings_(num_strings),
                                                alphabet_size_(26) {
        automaton_.reserve(2 << num_strings_ * 50);
        automaton_.push_back({.num_terminals = std::vector<bool>(false, num_strings),
                                     .parent = -1,
                                     .suffix_link = -1,
                                     .mask = std::vector<bool>(false, num_strings_),
                                     .switches = std::vector<int64_t>(-1, alphabet_size_),
                                     .go = std::vector<int64_t>(-1, alphabet_size_)
                             });
    }

    void AddString(const std::string &str, int num) {
        int64_t current_vertex = 0;
        for (size_t index = 0; index < str.size(); ++index) {
            int switches_index = str[index] - 'a';

            if (automaton_[current_vertex].switches[switches_index] == -1) {
                automaton_.push_back(
                        {.parent_switch = str[index], // TODO: .parent_switch = switches_index,  ???
                                .parent = current_vertex,
                                .suffix_link = -1,
                                .mask = std::vector<bool>(false, num_strings_),
                                .switches = std::vector<int64_t>(-1, alphabet_size_),
                                .go = std::vector<int64_t>(-1, alphabet_size_)
                        });
                automaton_[current_vertex].switches[switches_index] = size_++;
            }
            current_vertex = automaton_[current_vertex].switches[switches_index];
        }
        automaton_[current_vertex].num_terminals[num] = true;
    }

    void AddStrings(const std::vector<std::string> &strings) {
        int i = 0;
        for (auto &str: strings) {
            AddString(str, i++);
        }
    }

    int64_t GetSuffixLink(int64_t vertex) {
        // TODO: reimplement
        if (automaton_[vertex].suffix_link == -1) {
            if (vertex == 0 || automaton_[vertex].parent == 0) {
                automaton_[vertex].suffix_link = 0;
            } else {
                automaton_[vertex].suffix_link = Switch(GetSuffixLink(automaton_[vertex].parent),
                                                        automaton_[vertex].parent_switch);
            }
        }
        return automaton_[vertex].suffix_link;
    }

    int64_t Switch(int64_t vertex, char c) {
        // TODO: reimplement
        if (automaton_[vertex].go[c] == -1) {
            if (automaton_[vertex].switches[c] != -1) {
                automaton_[vertex].go[c] = automaton_[vertex].switches[c];
            } else {
                automaton_[vertex].go[c] = vertex == 0 ? 0 : Switch(GetSuffixLink(vertex), c);
            }
        }
        return automaton_[vertex].go[c];
    }


private:
    struct State {
        std::vector<bool> num_terminals;
        char parent_switch;
        int64_t parent;
        int64_t suffix_link;
        // TODO: do we really need mask? (Or just num_terminals)
        std::vector<bool> mask;
        std::vector<int64_t> switches;
        std::vector<int64_t> go;
    };
    int64_t size_;
    int num_strings_;
    int alphabet_size_;
    std::vector<State> automaton_;

    std::vector<bool> OperationOr(const std::vector<bool> &vec1, const std::vector<bool> &vec2) {
        std::vector<bool> result(vec1.size());
        std::transform(vec1.begin(), vec1.end(), vec2.begin(), result.begin(),
                       [](bool a, bool b) { return a || b; });
        return result;
    }
};


int main() {
    SetUp();
    int num_strings;
    std::cin >> num_strings;
    auto strings = ReadStrings(num_strings);
    MultiDimensionalCorasick automaton = MultiDimensionalCorasick(num_strings);
    // TODO: implement BFS (in alphabet order) with mask updating
}





