#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include <regex>
#include <unordered_map>
#include <unordered_set>

using namespace std;

// Custom functions
std::vector<std::string> split(const std::string &s, const char *delim);
void update_missing_counts(std::vector<std::string> line, unsigned long long **counts);
void update_col_tabs(std::vector<std::string> line,
                       std::vector<std::string> &col_names,
                       std::unordered_map<std::string, std::unordered_map<std::string, unsigned long long>> &col_tables);
void omop_update_cross_tabs(std::vector<std::string> line,
                       std::vector<std::string> &col_names,
                       std::unordered_map<std::string, std::unordered_map<std::string, unsigned long long>> &col_tables);
void omop_write_cross_tabs(std::unordered_map<std::string, std::unordered_map<std::string, unsigned long long>> col_tables,
                       std::vector<std::string> col_names,
                       std::string res_path,
                       std::string file_name);
void omop_write_cross_tabs(std::unordered_map<std::string, std::unordered_map<std::string, unsigned long long>> col_tables,
                       std::vector<std::string> col_names,
                       std::string res_path,
                       std::string file_name);
std::string concat_string(std::vector<std::string> elems, std::string sep = std::string(""));
void check_out_open(std::ofstream &file_stream, std::string file_path);
void check_in_open(std::ifstream &file_stream, std::string file_path);
void write_missing_res(unsigned long long **counts,
                       std::vector<std::string> col_names,
                       int n_cols,
                       std::string res_path,
                       std::string file_name);
void write_log(unsigned long long line_count, 
               unsigned long long total_line_count,
                       std::string res_path,
                       std::string file_name);