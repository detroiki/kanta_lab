#include "../header.h"

int main(int argc, char *argv[]) {
    std::string file_path = argv[1];
    std::string res_path = argv[2];

    // Counts
    int first_line = 1;

    std::unordered_map<std::string, std::unordered_set<std::string>> lab_indv_count;


    std::ofstream in_file;
    in_file.open(file_path); check_in_open(in_file, file_path); 

    // Lines
    std::string line;

    // In
    while(std::getline(in_file, line)) {
        if(first_line == 1) {
            first_line = 0;
            continue;
        }
        // 0: FINREGISTRYID, 1: DATE, 2: LAB_NAME, 3: ID, 4: ID_SOURCE, 5: NAME, 6: ABBREVIATION, 7: VALUE, 8: UNIT, 9: ABNORMALITY
        std::vector<std::string> line_vec = split(line, ";");

        std::string omop_id = line_vec[9];
        std::string finregistryid = line_vec[0];

        lab_indv_count[omop_id].insert(finregistryid);
    }

    in_file.close(); 

    std::unordered_set<std::string> keeping_omop_ids;
    for(auto omop_id: lab_indv_count) {
        if(omop_id.second.size() > 1000){
            keeping_omop_ids.insert(omop_id.first);
        }
    }


    // Out File
    std::vector<std::string> full_res_path_vec = {res_path, "processed/data/all_minimal_omop_top.csv"};    
    std::string full_res_path = concat_string(full_res_path_vec, std::string(""));

    std::ofstream res_file;
    res_file.open(full_res_path); check_out_open(res_file, full_res_path); 
}