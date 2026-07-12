#include "benchmarking.h"

#include <iostream>
#include <vector>
#include <string>
#include <chrono>


benchResults Benchmark::run_TrieBenchmarking(const std::vector<wordEntry>& entries){
benchResults results;
Trie trie;

// Build Time (ms) - Time is recorded before and after words are inserted and built

auto buildBegin = std::chrono::high_resolution_clock::now();

for(const auto& entry : entries){
	trie.insert(entry.first, entry.second);}

auto buildEnd = std::chrono::high_resolution_clock::now();
std::chrono::duration<double, std::milli> 
buildDuration = buildEnd - buildBegin;

results.buildTime = buildDuration.count();

// Mem usage
//For Trie: (Already done in trie file)
results.memoryUsage = trie.approxMemoryBytes();

// Query Time (With 1000 example prefixes) Time is recorded the same but divided by the # of prefixes 

std::vector<std::string> testingPrefixes;
if(entries.size()> 1000){ for (size_t i = 0; i<1000; ++i){
	const std::string& word = entries[i*(entries.size()/1000)].first;

if(word.length() >=3){
	testingPrefixes.push_back(word.substr(0,3));
}}
}

auto queryStart = std::chrono::high_resolution_clock::now();
for (const auto& prefix: testingPrefixes){
	auto searchResults = trie.search(prefix, 10);
}


auto queryEnd = std::chrono::high_resolution_clock::now();
std::chrono::duration<double, std::micro>
queryDuration = queryEnd - queryStart;

if (!testingPrefixes.empty()){
	results.avgQueryTime = queryDuration.count() / testingPrefixes.size();
}
else{ results.avgQueryTime = 0.0;
}

results.trie = std::move(trie);
return results;
}



// PLACEHOLDER FOR HASHTABLES



benchResults Benchmark::run_HashTableBenchmarking(const std::vector<wordEntry>& entries){
benchResults results;
HashTable hashtable;

auto buildBegin = std::chrono::high_resolution_clock::now();

for(const auto& entry : entries){
	hashtable.insert(entry.first, entry.second);}

auto buildEnd = std::chrono::high_resolution_clock::now();
std::chrono::duration<double, std::milli> 
buildDuration = buildEnd - buildBegin;

results.buildTime = buildDuration.count();

results.memoryUsage = hashtable.approxMemoryBytes();

std::vector<std::string> testingPrefixes;
if(entries.size()> 1000){ for (size_t i = 0; i<1000; ++i){
	const std::string& word = entries[i*(entries.size()/1000)].first;

if(word.length() >=3){
	testingPrefixes.push_back(word.substr(0,3));
}}
}

auto queryStart = std::chrono::high_resolution_clock::now();
for (const auto& prefix: testingPrefixes){
	auto searchResults = hashtable.search(prefix, 10);
}


auto queryEnd = std::chrono::high_resolution_clock::now();
std::chrono::duration<double, std::micro>
queryDuration = queryEnd - queryStart;
results.avgQueryTime = queryDuration.count() / testingPrefixes.size();

results.hashtable = std::move(hashtable);
return results;
}


