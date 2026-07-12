#ifndef DATASET_LOADER_H
#define DATASET_LOADER_H
#include <string>
#include <utility>
#include <vector>

using wordEntry = std::pair<std::string, long long>;
std::vector<wordEntry> loadDictionary(const std::string& filepath);


#endif // DATASET_LOADER_H
