#include "../header.h"

void get_lab_indv_counts(std::unordered_map<std::string, std::unordered_set<std::string>> &lab_indv_count,
                         std::string file_path) {
    // Opening file
    std::ifstream in_file;
    in_file.open(file_path); check_in_open(in_file, file_path); 

    // Lines
    std::string line;
    int first_line = 1; // Indicates header line

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
}