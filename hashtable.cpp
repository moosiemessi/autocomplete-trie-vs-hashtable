#include "hashtable.h"
#include <algorithm>
#include <cctype>

HashTable::HashTable() : buckets_(kInitialBucketCount) {}

std::string HashTable::toLower(const std::string& s) {
    std::string out = s;
    std::transform(out.begin(), out.end(), out.begin(),
                   [](unsigned char c) { return std::tolower(c); });
    return out;
}

// Polynomial rolling hash (base 31, the same scheme Java's String.hashCode uses). Cheap to compute, spreads lowercase-letter strings well, and is
// simple enough to reason about for the write-up.
size_t HashTable::hashString(const std::string& s) {
    size_t h = 0;
    for (unsigned char c : s) {
        h = h * 31 + c;
    }
    return h;
}

double HashTable::loadFactor() const {
    return buckets_.empty() ? 0.0
                             : static_cast<double>(wordCount_) / static_cast<double>(buckets_.size());
}

void HashTable::rehash(size_t newBucketCount) {
    std::vector<std::vector<Entry>> newBuckets(newBucketCount);
    for (auto& bucket : buckets_) {
        for (auto& entry : bucket) {
            size_t idx = hashString(entry.word) % newBucketCount;
            newBuckets[idx].push_back(std::move(entry));
        }
    }
    buckets_ = std::move(newBuckets);
}

void HashTable::insert(const std::string& word, long long frequency) {
    if (word.empty()) return;

    const std::string key = toLower(word);
    size_t idx = hashString(key) % buckets_.size();

    for (auto& entry : buckets_[idx]) {
        if (entry.word == key) {
            if (frequency > entry.frequency) entry.frequency = frequency;
            return; // already present, not a new word
        }
    }

    buckets_[idx].push_back(Entry{key, frequency});
    ++wordCount_;

    if (loadFactor() > kMaxLoadFactor) {
        rehash(buckets_.size() * 2);
    }
}

bool HashTable::contains(const std::string& word) const {
    if (word.empty()) return false;
    const std::string key = toLower(word);
    size_t idx = hashString(key) % buckets_.size();
    for (const auto& entry : buckets_[idx]) {
        if (entry.word == key) return true;
    }
    return false;
}

std::vector<std::string> HashTable::search(const std::string& prefix, size_t maxResults) const {
    const std::string key = toLower(prefix);

    std::vector<std::pair<std::string, long long>> matches;
    for (const auto& bucket : buckets_) {
        for (const auto& entry : bucket) {
            if (entry.word.compare(0, key.size(), key) == 0) {
                matches.emplace_back(entry.word, entry.frequency);
            }
        }
    }

    const bool hasFrequencies = std::any_of(
        matches.begin(), matches.end(),
        [](const auto& p) { return p.second > 0; });

    if (hasFrequencies) {
        std::sort(matches.begin(), matches.end(),
                  [](const auto& a, const auto& b) {
                      if (a.second != b.second) return a.second > b.second;
                      return a.first < b.first;
                  });
    } else {
        std::sort(matches.begin(), matches.end());
    }

    if (matches.size() > maxResults) {
        matches.resize(maxResults);
    }

    std::vector<std::string> results;
    results.reserve(matches.size());
    for (auto& p : matches) results.push_back(std::move(p.first));
    return results;
}

size_t HashTable::approxMemoryBytes() const {
    // rough estimate for comparing against the Trie, not exact
    constexpr size_t kBucketOverhead = sizeof(std::vector<Entry>);
    constexpr size_t kPerEntryOverhead = sizeof(Entry) + 32; // + small-string/heap slack
    return buckets_.size() * kBucketOverhead + wordCount_ * kPerEntryOverhead;
}
