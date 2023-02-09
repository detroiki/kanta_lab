#include "../header.h"

void write_col_counts(std::unordered_map<std::string, std::unordered_map<std::string, unsigned long long>> col_tables,
                       std::vector<std::string> col_names,
                       int n_cols,
                       std::string res_path,
                       std::string file_name) {
    // Opening results file
    for(auto col_name: col_names) {
        std::vector<std::string> full_res_path_vec = {res_path, "processed/reports/counts/", file_name, "_", col_name, "_table.tsv"};
        std::string full_res_path = concat_string(full_res_path_vec);
        std::ofstream res_file;
        res_file.open(full_res_path); check_out_open(res_file, full_res_path);
        
        // Writing
        for(const std::pair<const std::string, unsigned long long>& elem: col_tables[col_name]) {
               res_file << elem.first << "\t" << elem.second << "\n";
        }

        // Closing
        res_file.close();
    }
}
