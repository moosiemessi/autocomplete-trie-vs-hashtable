#ifndef TRIE_H
#define TRIE_H

#include <string>
#include <vector>
#include <memory>
#include <unordered_map>
#include <cstddef>

class Trie {
public:
    Trie();
    ~Trie() = default;

    Trie(const Trie&) = delete;
    Trie& operator=(const Trie&) = delete;
    Trie(Trie&&) = default;
    Trie& operator=(Trie&&) = default;

    // frequency is optional; if set, search() ranks by it
    void insert(const std::string& word, long long frequency = 0);

    bool contains(const std::string& word) const;

    // returns up to maxResults words starting with prefix
    std::vector<std::string> search(const std::string& prefix, size_t maxResults = 10) const;

    size_t wordCount() const { return wordCount_; }
    size_t nodeCount() const { return nodeCount_; }
    size_t approxMemoryBytes() const;

private:
    struct TrieNode {
        std::unordered_map<char, std::unique_ptr<TrieNode>> children;
        bool isEndOfWord = false;
        long long frequency = 0;
    };

    std::unique_ptr<TrieNode> root_;
    size_t wordCount_ = 0;
    size_t nodeCount_ = 0;

    const TrieNode* findNode(const std::string& prefix) const;
    void collect(const TrieNode* node, std::string& current,
                 std::vector<std::pair<std::string, long long>>& results) const;
    static std::string toLower(const std::string& s);
};

#endif // TRIE_H
