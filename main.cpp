#include "trie.h"
#include "dataset_loader.h"
#include "benchmarking.h"
#include <chrono>
#include <iostream>
#include <string>
#include <fstream>
#include "hashtable.h"

int main(){

// Loading from dictionary file
auto entries = loadDictionary("data/words_alpha.txt");

if(entries.empty()) { //If file can't load
	return 1;}



	
// Getting Benchmarking results and printing
benchResults trieResults = Benchmark::run_TrieBenchmarking(entries);

std::cout << "Trie Benchmarking Results \n";
std::cout << "Build Time:  " << trieResults.buildTime << " Milliseconds\n";
std::cout << "Memory Usage:  " << trieResults.memoryUsage / 1024 << " Kilobytes\n";
std::cout << "Average Query Time:  " << trieResults.avgQueryTime << " Microseconds\n\n";


// PLACEHOLDER FOR HASHTABLES

benchResults hashTableResults = Benchmark::run_HashTableBenchmarking(entries);

std::cout << "Hash Table Benchmarking Results \n";
std::cout << "Build Time:  " << hashTableResults.buildTime << " Milliseconds\n";
std::cout << "Memory Usage:  " << hashTableResults.memoryUsage / 1024 << " Kilobytes\n";
std::cout << "Average Query Time:  " << hashTableResults.avgQueryTime << " Microseconds\n\n";

// Printing Results to CSV for graphs
system("mkdir results 2> nul");

std::ofstream csv("results/benchmark_results.csv", std::ios::out | std::ios::trunc);
if (!csv.is_open()){
	std::cerr << "Csv can't open\n";
}else{
	csv << "method,buildmsTimes,memorySize,avgQueryTimes\n";

csv << "Trie," 
<< trieResults.buildTime << "," 
<< trieResults.memoryUsage / 1024 << ","
<< trieResults.avgQueryTime << "\n";

csv << "Hash Table,"
<< hashTableResults.buildTime << "," 
<< hashTableResults.memoryUsage / 1024 << ","
<< hashTableResults.avgQueryTime << "\n";
csv.flush();
csv.close();
}

// INTERACTIVE DEMO: AUTOCOMPLETE

std::cout << "(Autocomplete) Type any prefix.\n";
std::cout << "Type 'exit' to quit.\n\n";
std::string input;

while(true){
std::cout << "> ";
std::getline(std::cin, input);
if(input=="exit") break;
if(input.empty()) continue;

//Timing Query (microseconds)
auto queryBegin = std::chrono::high_resolution_clock::now();
std::vector<std::string> matches = trieResults.trie.search(input, 10);
auto queryEnd = std::chrono::high_resolution_clock::now();

std::chrono::duration<double, std::micro> 
queryDuration = queryEnd - queryBegin;

if(matches.empty()){
	std::cout << "No matches\n";}
	else{std::cout << matches.size() << " matches found in " << queryDuration.count() << "microseconds\n";
	for (const auto& word : matches){ std::cout << " " << word << "\n";
	}std::cout << "\n";}


}




return 0;
}