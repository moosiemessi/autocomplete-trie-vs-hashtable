#ifndef HASH_TABLE_H
#define HASH_TABLE_H

#include <string>
#include <vector>
#include <cstddef>

// Hash table with separate chaining, built from scratch. Exposes the same shape of API as Trie (insert/contains/search/counts)
// so the benchmark harness can treat both structures identically.
class HashTable {
public:
    HashTable();
    ~HashTable() = default;

    HashTable(const HashTable&) = delete;
    HashTable& operator=(const HashTable&) = delete;
    HashTable(HashTable&&) = default;
    HashTable& operator=(HashTable&&) = default;

    // frequency is optional; if set, search() ranks by it (same semantics as Trie::insert)
    void insert(const std::string& word, long long frequency = 0);

    // O(1) average: hash + walk one bucket's chain
    bool contains(const std::string& word) const;

    // NOTE: a hash table has no notion of "prefix" the way a Trie does.
    std::vector<std::string> search(const std::string& prefix, size_t maxResults = 10) const;

    size_t wordCount() const { return wordCount_; }
    size_t bucketCount() const { return buckets_.size(); }
    size_t approxMemoryBytes() const;

private:
    struct Entry {
        std::string word;
        long long frequency = 0;
    };

    std::vector<std::vector<Entry>> buckets_;
    size_t wordCount_ = 0;

    static constexpr size_t kInitialBucketCount = 1024;
    static constexpr double kMaxLoadFactor = 0.75;

    static std::string toLower(const std::string& s);
    static size_t hashString(const std::string& s);
    double loadFactor() const;
    void rehash(size_t newBucketCount);
};

#endif // HASH_TABLE_H
