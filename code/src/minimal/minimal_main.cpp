#include "../header.h"

int main(int argc, char *argv[]) {
    // File and delimiter
    const char *delim = ";";
    int n_cols = 25;

    std::string res_path = argv[1];
    std::string file = argv[2];
    std::string thl_sote_path = argv[3];
    std::string abbrv_path = argv[4];
    
    // Out File
    std::vector<std::string> full_res_path_vec = {res_path, "processed/data/", "all_minimal_file_", file, ".csv"};    
    std::string full_res_path = concat_string(full_res_path_vec, std::string(""));
    std::vector<std::string> report_path_vec = {res_path, "processed/reports/counts/row_counts/row_counts_file_", file, ".csv"};    
    std::string report_path = concat_string(report_path_vec, std::string(""));
    std::vector<std::string> error_path_vec = {res_path, "processed/reports/problem_rows/problem_rows_file_", file, ".csv"};    
    std::string error_path = concat_string(error_path_vec, std::string(""));

    // For duplicates
    std::unordered_map<std::string, int> all_dup_lines;

    if(file != "1") {
        std::ifstream duplines_file;
        int crnt_file_no = std::stoi(file);

        for(int file_no=1; file_no < crnt_file_no; file_no++) {
            std::vector<std::string> duplines_path_vec = {res_path, "processed/reports/problem_rows/duplines_", std::to_string(file_no), ".csv"};    
            std::string duplines_path = concat_string(duplines_path_vec, std::string(""));
            cout << duplines_path << endl;
            duplines_file.open(duplines_path); check_in_open(duplines_file, duplines_path);
            std::string line;
            while(std::getline(duplines_file, line)) {
                std::vector<std::string> line_vec = split(line, "\t");
                all_dup_lines[line_vec[0]] = 0;
            }
            duplines_file.close();
        }
    }

    // Getting thl sote lab name map
    std::string line;
    std::ifstream lab_file;
    std::unordered_map<std::string, std::string> lab_map;
    lab_file.open(thl_sote_path); check_in_open(lab_file, thl_sote_path);
    while(std::getline(lab_file, line)) {
        std::vector<std::string> line_vec = split(line, "\t");
        lab_map[line_vec[0]] = line_vec[1];
    }
    lab_file.close();

    // Getting official abbreviations map
    std::ifstream abbrv_file;
    std::unordered_map<std::string, std::string> abbrv_map;
    abbrv_file.open(abbrv_path); check_in_open(abbrv_file, abbrv_path);
    while(std::getline(abbrv_file, line)) {
        std::vector<std::string> line_vec = split(line, "\t");
        abbrv_map[line_vec[0]] = line_vec[1];
    }
    abbrv_file.close();

    std::ofstream error_file;
    std::ofstream res_file;
    res_file.open(full_res_path); error_file.open(error_path); check_out_open(res_file, full_res_path); check_out_open(error_file, error_path); 


    // Counts
    int skip_count = 0;
    int dup_count = 0;
    unsigned long long line_count = 0;
    unsigned long long total_line_count = 0;
    unsigned long long value_na_count = 0;

    int lines_invalid = 0; // 0: line is valid 1: line is invalid 2: both line and new line are invalid 3: line is invalid, but newline is valid

    // Lines
    std::string new_line;

    // In
    while(std::getline(std::cin, line)) {
        ++total_line_count;

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
            ++total_line_count;
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
        std::string laboratory_name;
        std::string lab_id;
        std::string lab_source;
        std::string abbrv;
        // Line or newline is valid
        if((lines_invalid == 0) | (lines_invalid == 3)) {
            if((line_count == 0)) {
                res_file << "FINREGISTRYID;DATE;LABORATORY;ID;ID_SOURCE;NAME;ABBREVIATION;VALUE;UNIT;ABNORMALITY\n"; //Removed abnormality for now
                ++line_count;
            } else {
                // Replacing different NA indicators with NA
                for(int elem_idx=0; elem_idx < n_cols; elem_idx++) {
                    // Replacing NAs
                    if((full_line_vec[elem_idx] == "Puuttuu") |
                        (full_line_vec[elem_idx] == "\"\"") | 
                        (full_line_vec[elem_idx] == "TYHJÄ") | 
                        (full_line_vec[elem_idx] == "_") |
                        ((full_line_vec[elem_idx] == "-1") & (elem_idx != 19))) { // -1 in value not considered NA
                        full_line_vec[elem_idx] = "NA";
                    }
                }
                // Mapping laboratory IDs to laboratory names
                if(lab_map.find(full_line_vec[6]) != lab_map.end()) {
                    laboratory_name = lab_map[full_line_vec[6]];
                } else {
                    laboratory_name = "NA";
                }

                // Getting lab ID and SOURCE

                if(full_line_vec[16] == "NA") {
                    lab_id = full_line_vec[15];
                    lab_source = "0";
                } else {
                    lab_id = full_line_vec[16];
                    lab_source = "1";
                }

                // Mapping lab IDs to abbreviations
                if(abbrv_map.find(lab_id) != abbrv_map.end()) {
                    abbrv = abbrv_map[lab_id];
                } else {
                    abbrv = "NA";
                }
                if((!((full_line_vec[19] == "NA") & (full_line_vec[22] == "NA"))) & (full_line_vec[3] != "NA") ) { // Have value and entry ID 
                    std::vector<std::string> dup_vec = {full_line_vec[1], full_line_vec[11], laboratory_name, lab_id, full_line_vec[14], full_line_vec[19], full_line_vec[20]};
                    std::string dup_line = concat_string(dup_vec, std::string(""));

                    if(all_dup_lines.find(dup_line) == all_dup_lines.end()) {
                        all_dup_lines[dup_line] = 1;
                        // Selecting all non-ID rows to reduce the size
                        // 1: Hetu
                        // 11: Tutkimusaika
                        // 14: Paikallinentutkimusnimike
                        // 15: Paikallinentutkimusnimikeid
                        // 16: Laboratoriotutkimusnimkeid
                        // 19: Tutkimustulosarvo
                        // 20: Tutkimustulosyksikkö
                        // 22: Tuloksenpoikkeavuus
                        res_file << full_line_vec[1] << ";" <<  full_line_vec[11] << ";" << laboratory_name << ";" << lab_id << ";" << lab_source << ";";
                        res_file  <<  full_line_vec[14] << ";" << abbrv << ";";
                        res_file << full_line_vec[19] << ";" << full_line_vec[20] << ";" <<  full_line_vec[22] << "\n";
                        ++line_count;
                    } else {
                        all_dup_lines[dup_line]++;
                        ++dup_count;
                    }
                } else {
                    if(full_line_vec[19] == "NA") ++value_na_count;           
                    lines_invalid = 1;
                }
            }  
        }
        // Line is invalid
        if((lines_invalid == 1) | (lines_invalid == 3)) {
            error_file << full_line_vec[1] << ";" <<  full_line_vec[11] << ";" << laboratory_name << ";" << lab_id << ";" << lab_source << ";";
            error_file  <<  full_line_vec[14] << ";" << abbrv << ";";
            error_file << full_line_vec[19] << ";" << full_line_vec[20] << ";" <<  full_line_vec[22] << "\n";
            ++skip_count;
        }  
        // Newline is also invalid
        if(lines_invalid == 2) {
            error_file << full_line_vec[1] << ";" <<  full_line_vec[11] << ";" << laboratory_name << ";" << lab_id << ";" << lab_source << ";";
            error_file  <<  full_line_vec[14] << ";" << abbrv << ";";
            error_file << full_line_vec[19] << ";" << full_line_vec[20] << ";" <<  full_line_vec[22] << "\n";            ++skip_count;
        }
    }

    std::ofstream report_file;
    std::ofstream duplines_file;
    report_file.open(report_path); 

    std::vector<std::string> duplines_path_vec = {res_path, "processed/reports/problem_rows/", "duplines_", file, ".csv"};    
    std::string duplines_path = concat_string(duplines_path_vec, std::string(""));
    duplines_file.open(duplines_path);
    check_out_open(report_file, report_path); check_out_open(duplines_file, duplines_path); 

    report_file << "All rows: " << total_line_count << "\n";
    report_file << "Usable rows: " << line_count << "\n";
    report_file << "Skipped rows: " << skip_count << "\n";
    report_file << "Duplicate rows: " << dup_count << "\n";
    report_file << "Missing value rows: " << value_na_count << "\n";
    report_file << endl;

    for(const std::pair<const std::string, int>& elem: all_dup_lines) {
        if(elem.second != 0) {
            duplines_file << elem.first << "\t" << elem.second << "\n";
        }
    }

    duplines_file.close();
    res_file.close(); 
    report_file.close();  
}