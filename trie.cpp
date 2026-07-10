#include "trie.h"
#include <algorithm>
#include <cctype>

Trie::Trie() : root_(std::make_unique<TrieNode>()) {
    nodeCount_ = 1;
}

std::string Trie::toLower(const std::string& s) {
    std::string out = s;
    std::transform(out.begin(), out.end(), out.begin(),
                   [](unsigned char c) { return std::tolower(c); });
    return out;
}

void Trie::insert(const std::string& word, long long frequency) {
    if (word.empty()) return;

    const std::string key = toLower(word);
    TrieNode* node = root_.get();

    for (char ch : key) {
        auto it = node->children.find(ch);
        if (it == node->children.end()) {
            auto newNode = std::make_unique<TrieNode>();
            TrieNode* raw = newNode.get();
            node->children.emplace(ch, std::move(newNode));
            ++nodeCount_;
            node = raw;
        } else {
            node = it->second.get();
        }
    }

    if (!node->isEndOfWord) {
        node->isEndOfWord = true;
        ++wordCount_;
    }
    if (frequency > node->frequency) {
        node->frequency = frequency;
    }
}

bool Trie::contains(const std::string& word) const {
    if (word.empty()) return false;
    const TrieNode* node = findNode(toLower(word));
    return node != nullptr && node->isEndOfWord;
}

const Trie::TrieNode* Trie::findNode(const std::string& prefix) const {
    const TrieNode* node = root_.get();
    for (char ch : prefix) {
        auto it = node->children.find(ch);
        if (it == node->children.end()) {
            return nullptr;
        }
        node = it->second.get();
    }
    return node;
}

void Trie::collect(const TrieNode* node, std::string& current,
                    std::vector<std::pair<std::string, long long>>& results) const {
    if (node->isEndOfWord) {
        results.emplace_back(current, node->frequency);
    }

    // sort children so results come out alphabetical
    std::vector<char> keys;
    keys.reserve(node->children.size());
    for (const auto& kv : node->children) keys.push_back(kv.first);
    std::sort(keys.begin(), keys.end());

    for (char ch : keys) {
        current.push_back(ch);
        collect(node->children.at(ch).get(), current, results);
        current.pop_back();
    }
}

std::vector<std::string> Trie::search(const std::string& prefix, size_t maxResults) const {
    const std::string key = toLower(prefix);
    const TrieNode* startNode = findNode(key);
    if (startNode == nullptr) {
        return {};
    }

    std::vector<std::pair<std::string, long long>> matches;
    std::string current = key;
    collect(startNode, current, matches);

    const bool hasFrequencies = std::any_of(
        matches.begin(), matches.end(),
        [](const auto& p) { return p.second > 0; });

    if (hasFrequencies) {
        std::sort(matches.begin(), matches.end(),
                  [](const auto& a, const auto& b) {
                      if (a.second != b.second) return a.second > b.second;
                      return a.first < b.first;
                  });
    }

    if (matches.size() > maxResults) {
        matches.resize(maxResults);
    }

    std::vector<std::string> results;
    results.reserve(matches.size());
    for (auto& p : matches) results.push_back(std::move(p.first));
    return results;
}

size_t Trie::approxMemoryBytes() const {
    // rough estimate for comparing against the hash table, not exact
    constexpr size_t kFixedNodeOverhead = sizeof(TrieNode);
    constexpr size_t kPerChildEntryOverhead = sizeof(char) + sizeof(std::unique_ptr<TrieNode>) + 32;
    size_t childEntries = nodeCount_ > 0 ? nodeCount_ - 1 : 0;
    return nodeCount_ * kFixedNodeOverhead + childEntries * kPerChildEntryOverhead;
}
