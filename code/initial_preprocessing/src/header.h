#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include <regex>
#include <unordered_map>

using namespace std;

// Custom functions
std::vector<std::string> split(const std::string &s, const char *delim);
void update_missing_counts(std::vector<std::string> line, unsigned long long **counts);
void write_missing_res(unsigned long long **counts, 
                       std::vector<std::string> col_names,
                       int n_cols);
void write_log(unsigned long long line_count, unsigned long long total_line_count);
void update_col_tables(std::vector<std::string> line,
                       std::vector<std::string> &col_names,
                       std::unordered_map<std::string, std::unordered_map<std::string, unsigned long long>> &col_tables);
void write_col_counts(std::unordered_map<std::string, std::unordered_map<std::string, unsigned long long>> col_tables, 
                      std::vector<std::string> col_names, 
                      int n_cols);
