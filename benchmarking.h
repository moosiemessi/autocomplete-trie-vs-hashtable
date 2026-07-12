#ifndef BENCHMARKING_H
#define BENCHMARKING_H
#include <vector>
#include <string>
#include <iostream>
#include "trie.h"
#include "hashtable.h"
#include "dataset_loader.h"

struct benchResults {

size_t memoryUsage; // Bytes
double buildTime;  // Milliseconds
double avgQueryTime; //Microseconds
Trie trie;
HashTable hashtable;
};



class Benchmark{
	public:
	static benchResults run_TrieBenchmarking(const std::vector<wordEntry>& entries);

	
	static benchResults run_HashTableBenchmarking(const std::vector<wordEntry>& entries);
};




#endif // BENCHMARKING_H

