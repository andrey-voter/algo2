#include <algorithm>
#include <cstring>
#include <deque>
#include <iostream>
#include <iterator>
#include <map>
#include <memory>
#include <queue>
#include <sstream>
#include <string>
#include <set>
#include <vector>

template<class Iterator>
class IteratorRange {
public:
    IteratorRange(Iterator begin, Iterator end) : begin_(begin), end_(end) {}

    Iterator begin() const { return begin_; }

    Iterator end() const { return end_; }

private:
    Iterator begin_, end_;
};

namespace traverses {

    template<class Vertex>
    Vertex PopQueue(std::queue<Vertex> &non_empty_queue) {
        Vertex vertex = non_empty_queue.front();
        non_empty_queue.pop();
        return vertex;
    }

    // Traverses the connected component in a breadth-first order
    // from the vertex 'origin_vertex'.
    // Refer to
    // https://goo.gl/0qYXzC
    // for the visitor events.
    template<class Vertex, class Graph, class Visitor>
    void BreadthFirstSearch(Vertex origin_vertex, const Graph &graph,
                            Visitor visitor) {
        visitor.DiscoverVertex(origin_vertex);
        std::set<Vertex> discovered_vertices{origin_vertex};
        std::queue<Vertex> queue{{origin_vertex}};

        while (!queue.empty()) {
            Vertex vertex = PopQueue(queue);
            visitor.ExamineVertex(vertex);

            for (auto edge: OutgoingEdges(graph, vertex)) {
                visitor.ExamineEdge(edge);
                Vertex target = GetTarget(graph, edge);
                if (!discovered_vertices.count(target)) {
                    visitor.DiscoverVertex(target);
                    discovered_vertices.insert(target);
                    queue.push(target);
                }
            }
        }
    }

    // See "Visitor Event Points" on
    // https://goo.gl/wtAl0y
    template<class Vertex, class Edge>
    class BfsVisitor {
    public:
        virtual void DiscoverVertex(Vertex /*vertex*/) {}

        virtual void ExamineEdge(const Edge & /*edge*/) {}

        virtual void ExamineVertex(Vertex /*vertex*/) {}

        virtual ~BfsVisitor() = default;
    };

} // namespace traverses

namespace aho_corasick {

    struct AutomatonNode {
        AutomatonNode() : suffix_link(nullptr), terminal_link(nullptr) {}

        // Stores ids of strings which are ended at this node.
        std::vector<size_t> terminated_string_ids;
        // Stores tree structure of nodes.
        std::map<char, AutomatonNode> trie_transitions;
        // Stores cached transitions of the automaton, contains
        // only pointers to the elements of trie_transitions.
        std::map<char, AutomatonNode *> automaton_transitions_cache;
        AutomatonNode *suffix_link;
        AutomatonNode *terminal_link;
    };

    // Returns a corresponding trie transition 'nullptr' otherwise.
    AutomatonNode *GetTrieTransition(AutomatonNode *node, char character) {
        if (node->trie_transitions.find(character) != node->trie_transitions.end()) {
            return &(node->trie_transitions.find(character)->second);
        }
        return nullptr;
    }

    // Returns an automaton transition, updates 'node->automaton_transitions_cache'
    // if necessary.
    // Provides constant amortized runtime.
    AutomatonNode *GetAutomatonTransition(AutomatonNode *node, const AutomatonNode *root, char character) {

        if (node->automaton_transitions_cache.find(character) != node->automaton_transitions_cache.end()) {
            return node->automaton_transitions_cache.find(character)->second;
        }

        auto transition = GetTrieTransition(node, character);
        if (transition) {
            node->automaton_transitions_cache[character] = transition;
            return transition;
        }

        auto original_node = node;
        while (node != root) {
            node = node->suffix_link;
            transition = GetTrieTransition(node, character);
            if (transition) {
                original_node->automaton_transitions_cache[character] = transition;
                return transition;
            }
        }

        original_node->automaton_transitions_cache[character] = node;
        return node;
    }

    namespace internal {

        class AutomatonGraph {
        public:
            struct Edge {
                Edge(AutomatonNode *source, AutomatonNode *target, char character)
                        : source(source), target(target), character(character) {}

                AutomatonNode *source;
                AutomatonNode *target;
                char character;
            };
        };

        std::vector<typename AutomatonGraph::Edge>
        OutgoingEdges(const AutomatonGraph & /*graph*/, AutomatonNode *vertex) {

            std::vector<AutomatonGraph::Edge> outgoing_edges;

            for (auto &transition: vertex->trie_transitions) {
                outgoing_edges.emplace_back(vertex, &transition.second, transition.first);
            }
            return outgoing_edges;
        }

        AutomatonNode *GetTarget(const AutomatonGraph & /*graph*/, const AutomatonGraph::Edge &edge) {
            return edge.target;
        }

        class SuffixLinkCalculator : public traverses::BfsVisitor<AutomatonNode *, AutomatonGraph::Edge> {
        public:
            explicit SuffixLinkCalculator(AutomatonNode *root) : root_(root) {}

            void ExamineVertex(AutomatonNode *node) override {
                if (node->suffix_link) {
                    return;
                }
                node->suffix_link = root_;
            }

            void ExamineEdge(const AutomatonGraph::Edge &edge) override {
                if (edge.source == root_) {
                    return;
                }
                edge.target->suffix_link = GetAutomatonTransition(edge.source->suffix_link, root_, edge.character);
            }

        private:
            AutomatonNode *root_;
        };

        class TerminalLinkCalculator : public traverses::BfsVisitor<AutomatonNode *, AutomatonGraph::Edge> {
        public:
            explicit TerminalLinkCalculator(AutomatonNode *root) : root_(root) {}

            void DiscoverVertex(AutomatonNode *node) override {
                if (node == root_ or node->suffix_link->terminated_string_ids.empty()) {
                    return;
                }
                node->terminal_link = node->suffix_link;
                auto terminated_string_ids_end = node->terminated_string_ids.end();
                auto suf_terminated_string_ids_begin = node->suffix_link->terminated_string_ids.begin();
                auto suf_terminated_string_ids_end = node->suffix_link->terminated_string_ids.end();
                node->terminated_string_ids.insert(terminated_string_ids_end, suf_terminated_string_ids_begin,
                                                   suf_terminated_string_ids_end);
            }

        private:
            AutomatonNode *root_;
        };

    } // namespace internal

    class NodeReference {
    public:
        NodeReference() : node_(nullptr), root_(nullptr) {}

        NodeReference(AutomatonNode *node, AutomatonNode *root) : node_(node), root_(root) {}

        NodeReference Next(char character) const {
            return NodeReference(GetAutomatonTransition(node_, root_, character), root_);
        }

        template<class Callback>
        void GenerateMatches(Callback on_match) const {
            for (size_t terminated_string_id: TerminatedStringIds()) {
                on_match(terminated_string_id);
            }
        }

        bool IsTerminal() const {
            return !node_->terminated_string_ids.empty();
        }

        explicit operator bool() const {
            return node_ != nullptr;
        }

        bool operator==(NodeReference other) const {
            return root_ == other.root_ and node_ == other.node_;
        }

    private:
        using TerminatedStringIterator = std::vector<size_t>::const_iterator;
        using TerminatedStringIteratorRange = IteratorRange<TerminatedStringIterator>;

        NodeReference TerminalLink() const {
            return NodeReference(node_->terminal_link, root_);
        }

        TerminatedStringIteratorRange TerminatedStringIds() const {
            return TerminatedStringIteratorRange(node_->terminated_string_ids.begin(),
                                                 node_->terminated_string_ids.end());
        }

        AutomatonNode *node_;
        AutomatonNode *root_;
    };

    class AutomatonBuilder;

    class Automaton {
    public:
        Automaton() = default;

        Automaton(const Automaton &) = delete;

        Automaton &operator=(const Automaton &) = delete;

        NodeReference Root() {
            return NodeReference(&root_, &root_);
        }

    private:
        AutomatonNode root_;

        friend class AutomatonBuilder;
    };

    class AutomatonBuilder {
    public:
        void Add(const std::string &string, size_t id) {
            words_.push_back(string);
            ids_.push_back(id);
        }

        std::unique_ptr<Automaton> Build() {
            auto automaton = std::make_unique<Automaton>();
            BuildTrie(words_, ids_, automaton.get());
            BuildSuffixLinks(automaton.get());
            BuildTerminalLinks(automaton.get());
            return automaton;
        }

    private:
        static void BuildTrie(const std::vector<std::string> &words,
                              const std::vector<size_t> &ids, Automaton *automaton) {
            for (size_t i = 0; i < words.size(); ++i) {
                AddString(&automaton->root_, ids[i], words[i]);
            }
        }

        static void AddString(AutomatonNode *root, size_t string_id,
                              const std::string &string) {
            auto node = root;
            for (char character: string) {
                node = &node->trie_transitions[character];
            }
            node->terminated_string_ids.push_back(string_id);
        }

        static void BuildSuffixLinks(Automaton *automaton) {
            internal::AutomatonGraph graph;
            internal::SuffixLinkCalculator suffix_link_calculator(&automaton->root_);
            traverses::BreadthFirstSearch(&automaton->root_, graph, suffix_link_calculator);
        }

        static void BuildTerminalLinks(Automaton *automaton) {
            internal::AutomatonGraph graph;
            internal::TerminalLinkCalculator terminal_link_calculator(&automaton->root_);
            traverses::BreadthFirstSearch(&automaton->root_, graph, terminal_link_calculator);
        }

        std::vector<std::string> words_;
        std::vector<size_t> ids_;
    };

} // namespace aho_corasick

// Consecutive delimiters are not grouped together and are deemed
// to delimit empty strings
template<class Predicate>
std::vector<std::string> Split(const std::string &string, Predicate is_delimiter) {
    std::vector<std::string> substrings;
    std::string token;
    for (char c: string) {
        if (is_delimiter(c)) {
            substrings.push_back(token);
            token = "";
        } else {
            token += c;
        }
    }
    substrings.push_back(token);
    return substrings;
}


// Wildcard is a character that may be substituted
// for any of all possible characters.
class WildcardMatcher {
public:
    WildcardMatcher() : number_of_words_(0), pattern_length_(0) {}

    explicit WildcardMatcher(const std::vector<std::string> &substrings) {
        number_of_words_ = substrings.size();
        aho_corasick::AutomatonBuilder builder;
        size_t substring_end = 0;
        for (const std::string &substring: substrings) {
            substring_end += substring.size();
            builder.Add(substring, substring_end++);
        }
        words_occurrences_by_position_.resize(substring_end);
        aho_corasick_automaton_ = builder.Build();
        Reset();
    }

    WildcardMatcher static BuildFor(const std::string &pattern, char wildcard) {
        std::vector<std::string> substrings = Split(pattern,
                                                    [wildcard](char character) { return character == wildcard; });
        return WildcardMatcher(substrings);
    }

    // Resets the matcher. Call allows to abandon all data which was already
    // scanned,
    // a new stream can be scanned afterward.
    void Reset() {
        words_occurrences_by_position_.assign(words_occurrences_by_position_.size(), 0);
        state_ = aho_corasick_automaton_->Root();
        UpdateWordOccurrencesCounters();
    }

    template<class Callback>
    void Scan(char character, Callback on_match) {
        state_ = state_.Next(character);
        ShiftWordOccurrencesCounters();
        UpdateWordOccurrencesCounters();
        if (words_occurrences_by_position_.back() == number_of_words_) {
            on_match();
        }
    }

private:
    void UpdateWordOccurrencesCounters() {
        auto &words = words_occurrences_by_position_;
        state_.GenerateMatches([&words](size_t id) { ++words[id]; });
    }

    void ShiftWordOccurrencesCounters() {
        words_occurrences_by_position_.pop_back();
        words_occurrences_by_position_.push_front(0);
    }

    // Storing only O(|pattern|) elements allows us
    // to consume only O(|pattern|) memory for matcher.
    std::deque<size_t> words_occurrences_by_position_;
    aho_corasick::NodeReference state_;
    size_t number_of_words_;
    size_t pattern_length_;
    std::unique_ptr<aho_corasick::Automaton> aho_corasick_automaton_;
};

std::string ReadString(std::istream &input_stream) {
    std::string string;
    input_stream >> string;
    return string;
}

void SetUp() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);
    std::cout.tie(nullptr);
}

// Returns positions of the first character of an every match.
std::vector<size_t> FindFuzzyMatches(const std::string &pattern_with_wildcards,
                                     const std::string &text, char wildcard) {

    WildcardMatcher matcher = WildcardMatcher::BuildFor(pattern_with_wildcards, wildcard);

    std::vector<size_t> found;
    found.reserve(text.size());
    for (size_t i = 0; i < text.size(); ++i) {
        size_t occurrence = i + 1 - pattern_with_wildcards.size();
        matcher.Scan(text[i], [&]() { found.push_back(occurrence); });
    }

    return found;
}

void Print(const std::vector<size_t> &sequence) {
    SetUp();

    std::cout << sequence.size() << std::endl;
    for (size_t el: sequence) {
        std::cout << el << " ";
    }
    std::cout << std::endl;
}