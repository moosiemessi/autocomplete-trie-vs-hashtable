#include "dataset_loader.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include <algorithm>
#include <cctype>

std::vector<wordEntry> loadDictionary(const std::string& filepath){
	//Open file and check for loading errors
std::ifstream file(filepath);

if(!file.is_open()){
	std::cerr << "ERROR: Can't open file" << std::endl;
	return {};

}


std::vector<wordEntry> entries;
std::string word;


while (file >> word){
std::transform(word.begin(), word.end(), word.begin(), [](unsigned char c){return std::tolower(c);});
entries.push_back({word, 0LL});

}

return entries;
}
