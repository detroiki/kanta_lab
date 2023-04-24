#include "../header.h"

void write_col_counts(std::unordered_map<std::string, std::unordered_map<std::string, unsigned long long>> col_tables,
                       std::vector<std::string> col_names,
                       std::string res_path,
                       std::string file_name) {
    // Opening results file
    for(auto col_name: col_names) {
        if(col_tables[col_name].size() > 1) {
            std::vector<std::string> full_res_path_vec = {res_path, "processed/reports/counts/col_counts/", file_name, "_", col_name, "_table.tsv"};
            std::string full_res_path = concat_string(full_res_path_vec);
            std::ofstream res_file;
            res_file.open(full_res_path); check_out_open(res_file, full_res_path);
            
            // Writing
            if(col_name == "FINREGISTRYID") {
                res_file << "FINREGISTRYID\tDATE_TIME\tLAB_ID\tOMOP_ID\tCOUNT\n";
            } else if(col_name == "SERVICE_PROVIDER") {
                res_file << "SERVICE_PROVIDER\tCOUNT\n";
            } else if(col_name == "LAB_ID") {
                res_file << "LAB_ID\tCOUNT\n";
            } else if(col_name == "LAB_ABBREVIATION") {
                res_file << "LAB_ABBREVIATION\tCOUNT\n";
            } else if(col_name == "LAB_VALUE") {
                res_file << "LAB_ID\nOMOP_ID\tLAB_ABBREVIATION\tLAB_VALUE\tLAB_UNIT\tCOUNT\n";
            } else if(col_name == "LAB_UNIT") {
                res_file << "LAB_UNIT\tCOUNT\n";
            } else if(col_name == "LAB_ABNORMALITY") {
                res_file << "LAB_ABNORMALITY\tCOUNT\n";
            } else if(col_name == "OMOP_ID") {
                res_file << "LAB_ID\tOMOP_ID\tCOUNT\n";
            } else if(col_name == "OMOP_NAME") {
                res_file << "OMOP_ID\tOMOP_ABBREVIATION\tLAB_VALUE\tOMOP_UNIT\tSERVICE_PROVIDER\tCOUNT\n";
            } else if(col_name == "OMOP_ABBREVIATION") {
                res_file << "LAB_ABBREVIATION\tOMOP_ABBREVIATION\tCOUNT\n";
            } else if(col_name == "OMOP_UNIT") {
                res_file << "LAB_UNIT\tOMOP_UNIT\tCOUNT\n";
            }
            for(const std::pair<const std::string, unsigned long long>& elem: col_tables[col_name]) {
                res_file << elem.first << "\t" << elem.second << "\n";
            }

            // Closing
            res_file.close();
        }
    }
}
