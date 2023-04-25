#include "../header.h"

void omop_write_cross_tabs(std::unordered_map<std::string, std::unordered_map<std::string, unsigned long long>> col_tables,
                       std::vector<std::string> col_names,
                       std::string res_path,
                       std::string file_name) {
    // Going through each column in the file
    for(auto col_name: col_names) {
        // Checking if we made a table of the column
        if(col_tables[col_name].size() > 1) {
            // File preparation
            std::ofstream res_file;
            std::vector<std::string> full_res_path_vec = {res_path, "processed/reports/counts/col_counts/", file_name, "_"};

            // LAB_ID vs OMOP_ID
            if(col_name == "OMOP_ID") {
                // Opening
                full_res_path_vec.push_back("lab_id_omop_id_counts.tsv");
                std::string full_res_path = concat_string(full_res_path_vec);
                res_file.open(full_res_path); check_out_open(res_file, full_res_path);
                // Writing header
                res_file << "LAB_ID\tOMOP_ID\tCOUNT\n";

            // OMOP_ID vs OMOP_ABBREVIATION, LAB_VALUE, OMOP_UNIT, SERVICE_PROVIDER
            } else if(col_name == "OMOP_NAME") {
                // Opening
                full_res_path_vec.push_back("OMOP_ID_value_unit_service_provider_counts.tsv");
                std::string full_res_path = concat_string(full_res_path_vec);
                res_file.open(full_res_path); check_out_open(res_file, full_res_path);
                // Writing header
                res_file << "OMOP_ID\tOMOP_ABBREVIATION\tLAB_VALUE\tOMOP_UNIT\tSERVICE_PROVIDER\tCOUNT\n";

            // LAB_ABBREVIATION vs OMOP_ABBREVIATION
            } else if(col_name == "OMOP_ABBREVIATION") {
                // Opening
                full_res_path_vec.push_back("lab_abbreviation_omop_abbreviation_counts.tsv");
                std::string full_res_path = concat_string(full_res_path_vec);
                res_file.open(full_res_path); check_out_open(res_file, full_res_path);
                // Writing header
                res_file << "LAB_ABBREVIATION\tOMOP_ABBREVIATION\tCOUNT\n";

            // LAB_UNIT vs OMOP_UNIT
            } else if(col_name == "OMOP_UNIT") {
                // Opening
                full_res_path_vec.push_back("lab_unit_omop_unit_counts.tsv");
                std::string full_res_path = concat_string(full_res_path_vec);
                res_file.open(full_res_path); check_out_open(res_file, full_res_path);
                // Writing header
                res_file << "LAB_UNIT\tOMOP_UNIT\tCOUNT\n";
            }
            // Writing table
            for(const std::pair<const std::string, unsigned long long>& elem: col_tables[col_name]) {
                res_file << elem.first << "\t" << elem.second << "\n";
            }

            // Closing
            res_file.close();
        }
    }
}
