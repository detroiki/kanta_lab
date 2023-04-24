#include "../header.h"

int main(int argc, char *argv[]) {
    // File and delimiter
    const char *delim = ";";

    std::string res_path = argv[1];
    std::string file_name = argv[2];
    
    // Out File
    std::vector<std::string> full_res_path_vec = {res_path, "processed/reports/problem_rows/", file_name, ".cvs"};    
    std::string full_res_path = concat_string(full_res_path_vec, std::string(""));

    // For duplicates
    std::unordered_map<std::string, int> all_dup_lines;

    std::ofstream res_file;
    res_file.open(full_res_path);  check_out_open(res_file, full_res_path);; 


    // Counts
    unsigned long long total_line_count = 0;

    // In
    std::string line;
    while(std::getline(std::cin, line)) {
        ++total_line_count;
        // Split values and copy into resulting vector
        std::vector<std::string> line_vec = split(line, delim);
        std::string crnt_line = concat_string(line_vec, std::string(";"));

        if(all_dup_lines.find(crnt_line) == all_dup_lines.end()) {
            all_dup_lines[crnt_line] = 1;
            res_file << crnt_line << "\n";
        } else {
            all_dup_lines[crnt_line]++;
        }
    }

    res_file.close(); 
}