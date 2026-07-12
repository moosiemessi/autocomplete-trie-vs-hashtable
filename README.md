# AutoComplete Showdown - Trie V.S. Hash Table

### Team Hash Tries
- Francisco Rodriguez: Github (moosiemessi)
- Katie Gray: Github (KatieGrays)
- E Vanegas: Github (HalfHuesos)

### Welcome to Autocomplete Showdown!
This is a project built with C++ that showcases the the benchmarking differences between two different structures - Hash Tables and Tries.
In this, you will be able to interact and type in prefixes for words and matches from a dictionary will appear.
This project also contains applications in which results will be generated into graph plots.

### How to use autocomplete machine
1. Compile program (g++ -std=c++17 src/main.cpp src/benchmarking.cpp src/trie.cpp src/dataset_loader.cpp src/hashtable.cpp -o autocomplete.exe)
2. Run program (./autocomplete) You are limited to 10 results for each search
3. Type exit to exit

Instructions for use will also be provided once program runs.

### How to generate graphs
1. Install Python 3
2. Run (.\autocomplete.exe) and exit
3. Install matplotlib (pip install matplotlib)
4. run python scripts/generate_charts.py