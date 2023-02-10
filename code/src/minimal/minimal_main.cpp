#include "../header.h"

int main(int argc, char *argv[]) {
    // File and delimiter
    const char *delim = ";";
    // Defining zero as reading whole file here
    unsigned long long n_rows = 0;
    int n_cols = 25;

    std::string res_path = argv[1];
    std::vector<std::string> file_names;
    for(int file_idx = 2; file_idx < argc; file_idx++) {
        file_names.push_back(argv[file_idx]);
    }
    // Out File
    std::vector<std::string> full_res_path_vec = {res_path, "processed/data/", "all_minimal.csv"};    
    std::string full_res_path = concat_string(full_res_path_vec, std::string(""));
    std::vector<std::string> report_path_vec = {res_path, "processed/reports/", "all_file_row_counts.csv"};    
    std::string report_path = concat_string(report_path_vec, std::string(""));

    std::ofstream res_file;
    std::ofstream report_file;
    report_file.open(report_path); res_file.open(full_res_path); 
    check_out_open(report_file, report_path); check_out_open(res_file, full_res_path); 

    // Counts
    int skip_count = 0;
    int dup_count = 0;
    unsigned long long line_count = 0;
    unsigned long long total_line_count = 0;
    int lines_invalid = 0; // 0: line is valid 1: line is invalid 2: both line and new line are invalid 3: line is invalid, but newline is valid

    // Lines
    std::string line;
    std::string new_line;

    // For duplicates
    std::unordered_set<std::string> entry_id;

    // In
    std::ifstream in_file;
    for(auto file_name: file_names) {
        cout << "Reading: " << file_name << endl;
        // Opening file
        in_file.open(file_name);
        check_in_open(in_file, file_name);

        while(std::getline(in_file, line)) {
        if((line_count > n_rows) & (n_rows != 0)) {
                break;
            }
            // Split values and copy into resulting vector
            std::vector<std::string> full_line_vec(n_cols);
            std::vector<std::string> line_vec = split(line, delim);
                
            // Seemingly correctly defined line
            if(int(line_vec.size()) == n_cols) {
                std::copy(line_vec.begin(), line_vec.end(), full_line_vec.begin());
                lines_invalid = 0;
            // Too many columns, can't fix at the moment. Line invalid. TODO
            } else if(int(line_vec.size()) > n_cols) {
                lines_invalid = 1;
            // Trying to fix line potential early line breaks
            } else if(int(line_vec.size()) < n_cols)  {
                // Getting next line
                std::getline(in_file, new_line);
                std::vector<std::string> new_line_vec = split(new_line, delim);

                // New line is likely continuation of previous line
                if(int(new_line_vec.size()) < n_cols) {
                    // Concatinating back last element of previous line with first of new line
                    std::string new_name = concat_string(std::vector<std::string> {line_vec[line_vec.size()-1], new_line_vec[0]});
                    line_vec.at(line_vec.size()-1) =  new_name;

                    // Concatinating the two line vectors
                    line_vec.insert(line_vec.end(), next(new_line_vec.begin()), new_line_vec.end());
                    if(int(line_vec.size()) == n_cols) {
                        std::copy(line_vec.begin(), line_vec.end(), full_line_vec.begin());
                        lines_invalid = 0;
                    // Concatination does not give us a valid line. Both are invalid. 
                    } else {
                        lines_invalid = 2;
                    }
                // New line is actually a full line. Line is invalid. Newline is valid, using it. 
                } else if(int(new_line_vec.size()) == n_cols) { 
                    std::copy(new_line_vec.begin(), new_line_vec.end(), full_line_vec.begin());
                    lines_invalid = 3;
                } else {
                    lines_invalid = 2;
                }
            } 
            // Line or newline is valid
            if((lines_invalid == 0) | (lines_invalid == 3)) {
                // Replacing different NA indicators with NA
                for(int elem_idx=0; elem_idx < n_cols; elem_idx++) {
                    // Replacing NAs
                    if((full_line_vec[elem_idx] == "Puuttuu") |
                        (full_line_vec[elem_idx] == "\"\"") | 
                        (full_line_vec[elem_idx] == "_") |
                        ((full_line_vec[elem_idx] == "-1") & (elem_idx != 19))) { // -1 in value not considered NA
                        full_line_vec[elem_idx] = "NA";
                    }
                }
                if(entry_id.insert(full_line_vec[3]).second) {
                    // Selecting all non-ID rows to reduce the size
                    // 1: Hetu
                    // 11: Tutkimusaika
                    // 14: Paikallinentukkimusnimike
                    // 15: Paikallinentutkimusnimikeid
                    // 16: Laboratoriotutkimusnimkeid
                    // 19: Tutkimustulosarvo
                    // 20: Tutkimustulosyksikkö
                    // 22: Tuloksenvalmistumisaika
                    res_file << full_line_vec[1] << ";" <<  full_line_vec[11].substr(0, 10) << ";" <<  full_line_vec[14] << ";" <<  full_line_vec[15] << ";" <<  full_line_vec[16] << ";" << full_line_vec[19] << ";" << full_line_vec[20] << ";" <<  full_line_vec[22] << "\n";
                    line_count++;
                    total_line_count++;
                } else {
                    dup_count++;
                    total_line_count++;
                }
            } 
            // Line is invalid
            if((lines_invalid == 1) | (lines_invalid == 3)) {
                skip_count++;
                total_line_count++;
            }  
            // Newline is also invalid
            if(lines_invalid == 2) {
                skip_count++;
                total_line_count++;
            }
        }
        in_file.close();
    }

    report_file << "All rows: " << total_line_count << "\n";
    report_file << "Usable rows: " << line_count << "\n";
    report_file << "Skipped rows: " << skip_count << "\n";
    report_file << "Duplicate rows: " << dup_count << endl;

    // Closing
    res_file.close();
    report_file.close();
}