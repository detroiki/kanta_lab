#include "../header.h"

int main(int argc, char *argv[]) {
    // File and delimiter
    const char *delim = ";";
    // Defining zero as reading whole file here
    unsigned long long n_rows = 0;
    int n_cols = 25;

    std::string res_path = argv[1];
    std::string file = "file_1";
    if(argc == 3)
        file = argv[2];
    

    // Out File
    std::vector<std::string> full_res_path_vec = {res_path, "processed/data/", "minimal_", file, ".csv"};    
    std::string full_res_path = concat_string(full_res_path_vec, std::string(""));
    std::vector<std::string> report_path_vec = {res_path, "processed/reports/", "all_file_row_counts.csv"};    
    std::string report_path = concat_string(report_path_vec, std::string(""));
    std::vector<std::string> entryids_path_vec = {res_path, "processed/reports/", "entryids.csv"};    
    std::string entryids_path = concat_string(entryids_path_vec, std::string(""));

    // For duplicates
    std::unordered_set<std::string> entry_ids;

    if(file != "file_1") {
        std::ifstream entryids_file;
        entryids_file.open(entryids_path);
        check_in_open(entryids_file, entryids_path);
        std::string line;
        while(std::getline(entryids_file, line)) {
            entry_ids.insert(line);
        }
    }

    std::ofstream res_file;
    res_file.open(full_res_path);
    check_out_open(res_file, full_res_path); 

    // Counts
    int skip_count = 0;
    int dup_count = 0;
    unsigned long long line_count = 0;
    unsigned long long total_line_count = 0;

    int lines_invalid = 0; // 0: line is valid 1: line is invalid 2: both line and new line are invalid 3: line is invalid, but newline is valid

    // Lines
    std::string line;
    std::string new_line;

    // In
    while(std::getline(std::cin, line)) {
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
            std::getline(std::cin, new_line);
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
            if(line_count == 0) {
                res_file << "FINNREGISTRYID;DATE;ID;ID_SOURCE;NAME;VALUE;UNIT;ABNORMALITY\n";
                line_count++;
                total_line_count++;
            } else {
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
                if(entry_ids.insert(full_line_vec[3]).second) {
                    // Selecting all non-ID rows to reduce the size
                    // 1: Hetu
                    // 11: Tutkimusaika
                    // 14: Paikallinentukkimusnimike
                    // 15: Paikallinentutkimusnimikeid
                    // 16: Laboratoriotutkimusnimkeid
                    // 19: Tutkimustulosarvo
                    // 20: Tutkimustulosyksikkö
                    // 22: Tuloksenpoikkeavuus
                    res_file << full_line_vec[1] << ";" <<  full_line_vec[11].substr(0, 10) << ";";
                    if(full_line_vec[16] == "NA") {
                        res_file << full_line_vec[15] << ";0;";
                    } else {
                        res_file << full_line_vec[16] << ";1;";
                    }
                    res_file  <<  full_line_vec[14] << ";" << full_line_vec[19] << ";" << full_line_vec[20] << ";" <<  full_line_vec[22] << "\n";
                    line_count++;
                    total_line_count++;
                } else {
                    dup_count++;
                    total_line_count++;
                }
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

    std::ofstream report_file;
    std::ofstream entryids_file;
    if(file != "file_1") {
        report_file.open(report_path, std::ios_base::app); 
    } else {
        report_file.open(report_path); 
    }
    entryids_file.open(entryids_path);
    check_out_open(report_file, report_path); check_out_open(entryids_file, entryids_path); 

    report_file << file << "\n";
    report_file << "All rows: " << total_line_count << "\n";
    report_file << "Usable rows: " << line_count << "\n";
    report_file << "Skipped rows: " << skip_count << "\n";
    report_file << "Duplicate rows: " << dup_count << "\n";
    report_file << endl;

    for(auto entry_id: entry_ids) {
        entryids_file << entry_id << "\n";
    }

    if(file != "file_1") {
        report_file.close(); 
    }
    res_file.close(); 
}