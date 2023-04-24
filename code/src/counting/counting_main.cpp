#include "../header.h"

int main(int argc, char *argv[]) {
     // File and delimiter
    const char *delim = ";";

    std::string file_name = argv[1];
    std::string res_path = argv[2];

    // Opening
    std::ifstream in_file;


    // Defining result variables
    std::vector<std::string> col_names;
    // Table of each value for the different columns
    std::unordered_map<std::string, std::unordered_map<std::string, unsigned long long>> col_tables; 
    unsigned long long line_count = 0;
    unsigned long long total_line_count = 0;

    // Reading file line by line
    std::string line;
    while(std::getline(std::cin, line)) {
        //cout << line << endl;
        total_line_count++;
        // Split values and copy into resulting vector
        std::vector<std::string> line_vec = split(line, delim);
        // Column names
        if(line_count == 0) {
            // Copying the first line elements into the column vector
            std::copy(line_vec.begin(), line_vec.end(), std::back_inserter(col_names));
        } else {
            update_col_tables(line_vec, col_names, col_tables);
        }
        line_count++;
    } 
    in_file.close();
    // Writing results
    write_col_counts(col_tables, col_names, res_path, file_name);
}

