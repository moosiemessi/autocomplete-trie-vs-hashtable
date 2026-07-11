#include "hashtable.h"
#include <cassert>
#include <chrono>
#include <fstream>
#include <iostream>

static void runCorrectnessTests() {
    HashTable table;

    assert(table.wordCount() == 0);
    assert(table.search("a").empty());
    assert(!table.contains("anything"));

    table.insert("cat");
    table.insert("car");
    table.insert("card");
    table.insert("care");
    table.insert("dog");
    assert(table.contains("cat"));
    assert(table.contains("card"));
    assert(!table.contains("ca"));
    assert(!table.contains("caravan"));

    table.insert("Apple");
    assert(table.contains("apple"));
    assert(table.contains("APPLE"));

    auto results = table.search("ca", 10);
    assert(results.size() == 4);
    assert(results[0] == "car");
    assert(results[1] == "card");
    assert(results[2] == "care");
    assert(results[3] == "cat");

    auto capped = table.search("ca", 2);
    assert(capped.size() == 2);
    assert(capped[0] == "car");
    assert(capped[1] == "card");

    assert(table.search("xyz").empty());

    auto all = table.search("", 100);
    assert(all.size() == 6);

    table.insert("cat"); // duplicate, should not increase wordCount
    assert(table.wordCount() == 6);

    HashTable ranked;
    ranked.insert("the", 1000);
    ranked.insert("theory", 50);
    ranked.insert("theater", 200);
    auto rankedResults = ranked.search("the", 10);
    assert(rankedResults.size() == 3);
    assert(rankedResults[0] == "the");
    assert(rankedResults[1] == "theater");
    assert(rankedResults[2] == "theory");

    assert(table.approxMemoryBytes() > 0);
    assert(table.bucketCount() > 0);

    std::cout << "All correctness tests passed.\n";
}

static void runRehashTest() {
    // Insert enough words to force at least one rehash and confirm nothing gets lost or duplicated in the process
    HashTable table;
    const size_t n = 5000;
    for (size_t i = 0; i < n; ++i) {
        table.insert("word" + std::to_string(i));
    }
    assert(table.wordCount() == n);
    for (size_t i = 0; i < n; ++i) {
        assert(table.contains("word" + std::to_string(i)));
    }
    std::cout << "Rehash test passed. Final bucket count: " << table.bucketCount() << "\n";
}

static void runBuiltInDemo() {
    std::cout << "\n--- Built-in demo ---\n";
    HashTable table;
    for (const std::string w : {"apple", "app", "application", "apply",
                                 "banana", "band", "bandana", "bandwidth"}) {
        table.insert(w);
    }
    for (const std::string prefix : {"app", "band", "ba", "xyz"}) {
        auto results = table.search(prefix, 10);
        std::cout << "search(\"" << prefix << "\") -> [";
        for (size_t i = 0; i < results.size(); ++i) {
            std::cout << results[i] << (i + 1 < results.size() ? ", " : "");
        }
        std::cout << "]\n";
    }
    std::cout << "words: " << table.wordCount()
              << ", buckets: " << table.bucketCount()
              << ", approxMemoryBytes: " << table.approxMemoryBytes() << "\n";
}

int main() {
    runCorrectnessTests();
    runRehashTest();
    runBuiltInDemo();
    return 0;
}
