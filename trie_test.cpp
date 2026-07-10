
#include "trie.h"
#include <cassert>
#include <chrono>
#include <fstream>
#include <iostream>
#include <sstream>

static void runCorrectnessTests() {
    Trie trie;

    assert(trie.wordCount() == 0);
    assert(trie.search("a").empty());
    assert(!trie.contains("anything"));

    trie.insert("cat");
    trie.insert("car");
    trie.insert("card");
    trie.insert("care");
    trie.insert("dog");
    assert(trie.contains("cat"));
    assert(trie.contains("card"));
    assert(!trie.contains("ca"));
    assert(!trie.contains("caravan"));

    trie.insert("Apple");
    assert(trie.contains("apple"));
    assert(trie.contains("APPLE"));

    auto results = trie.search("ca", 10);
    assert(results.size() == 4);
    assert(results[0] == "car");
    assert(results[1] == "card");
    assert(results[2] == "care");
    assert(results[3] == "cat");

    auto capped = trie.search("ca", 2);
    assert(capped.size() == 2);
    assert(capped[0] == "car");
    assert(capped[1] == "card");

    assert(trie.search("xyz").empty());

    auto all = trie.search("", 100);
    assert(all.size() == 6);

    trie.insert("cat");
    assert(trie.wordCount() == 6);

    Trie ranked;
    ranked.insert("the", 1000);
    ranked.insert("theory", 50);
    ranked.insert("theater", 200);
    auto rankedResults = ranked.search("the", 10);
    assert(rankedResults.size() == 3);
    assert(rankedResults[0] == "the");
    assert(rankedResults[1] == "theater");
    assert(rankedResults[2] == "theory");

    assert(trie.nodeCount() > trie.wordCount());
    assert(trie.approxMemoryBytes() > 0);

    std::cout << "All correctness tests passed.\n";
}

static void runBuiltInDemo() {
    std::cout << "\n--- Built-in demo ---\n";
    Trie trie;
    for (const std::string w : {"apple", "app", "application", "apply",
                                 "banana", "band", "bandana", "bandwidth"}) {
        trie.insert(w);
    }
    for (const std::string prefix : {"app", "band", "ba", "xyz"}) {
        auto results = trie.search(prefix, 10);
        std::cout << "search(\"" << prefix << "\") -> [";
        for (size_t i = 0; i < results.size(); ++i) {
            std::cout << results[i] << (i + 1 < results.size() ? ", " : "");
        }
        std::cout << "]\n";
    }
    std::cout << "words: " << trie.wordCount()
              << ", nodes: " << trie.nodeCount()
              << ", approxMemoryBytes: " << trie.approxMemoryBytes() << "\n";
}

static void runFileDemo(const std::string& path) {
    std::ifstream in(path);
    if (!in) {
        std::cerr << "Could not open " << path << "\n";
        return;
    }

    Trie trie;
    std::string line;
    auto start = std::chrono::steady_clock::now();
    size_t count = 0;
    while (std::getline(in, line)) {
        if (line.empty()) continue;
        // supports "word" or "word,frequency" lines
        auto comma = line.find(',');
        if (comma == std::string::npos) {
            trie.insert(line);
        } else {
            std::string word = line.substr(0, comma);
            long long freq = 0;
            try { freq = std::stoll(line.substr(comma + 1)); } catch (...) {}
            trie.insert(word, freq);
        }
        ++count;
    }
    auto end = std::chrono::steady_clock::now();
    double buildMs = std::chrono::duration<double, std::milli>(end - start).count();

    std::cout << "Loaded " << count << " lines in " << buildMs << " ms\n";
    std::cout << "words: " << trie.wordCount()
              << ", nodes: " << trie.nodeCount()
              << ", approxMemoryBytes: " << trie.approxMemoryBytes() << "\n";

    std::cout << "\nType a prefix (empty line to quit):\n";
    std::string prefix;
    while (std::getline(std::cin, prefix)) {
        if (prefix.empty()) break;
        auto qStart = std::chrono::steady_clock::now();
        auto results = trie.search(prefix, 10);
        auto qEnd = std::chrono::steady_clock::now();
        double queryUs = std::chrono::duration<double, std::micro>(qEnd - qStart).count();

        std::cout << results.size() << " match(es) in " << queryUs << " us: ";
        for (size_t i = 0; i < results.size(); ++i) {
            std::cout << results[i] << (i + 1 < results.size() ? ", " : "");
        }
        std::cout << "\n> ";
    }
}

int main(int argc, char** argv) {
    runCorrectnessTests();
    runBuiltInDemo();

    if (argc > 1) {
        runFileDemo(argv[1]);
    }

    return 0;
}
