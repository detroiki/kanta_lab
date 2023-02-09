#include "../header.h"

void write_missing_res(unsigned long long **counts,
                       std::vector<std::string> col_names,
                       int n_cols,
                       std::string res_path,
                       std::string file_name) {
    // Opening results file
    std::vector<std::string> res_path_vec {res_path, "processed/reports/missing/", file_name, "_missing_report.tsv"};
    std::string full_res_path = concat_string(res_path_vec);
    ofstream res_file;
    res_file.open(full_res_path);
    check_out_open(res_file, full_res_path);
    cout << full_res_path << endl;
    // Column and Row informations
    std::vector<std::string> missing_types = { "Puuttuu", "-1", "Empty", "NA"};
    res_file << "Type\t";
    for(int col_idx=0; col_idx < n_cols; col_idx++) {
        res_file << col_names[col_idx] << "\t";
    }
    res_file << "\n";

    // Writing counts
    for(int row_idx=0; row_idx < 4; row_idx++) {
        res_file << missing_types[row_idx] << "\t";
        for(int col_idx=0; col_idx < n_cols; col_idx++) {
            res_file << counts[row_idx][col_idx] << "\t";
        }
        res_file << "\n";
    }

    // Closing
    res_file.close();
}

void write_log(unsigned long long line_count, 
               unsigned long long total_line_count,
                       std::string res_path,
                       std::string file_name) {
    std::vector<std::string> res_path_vec {res_path, "processed/reports/missing/", file_name, "_missing.txt"};
    std::string full_res_path = concat_string(res_path_vec);
    ofstream res_file;
    res_file.open(full_res_path);
    check_out_open(res_file, full_res_path);

    res_file << "Total number of rows: " << total_line_count-1 << "\n" << "Number of used rows: " << line_count-1 << endl;

    // Closing
    res_file.close();
}
