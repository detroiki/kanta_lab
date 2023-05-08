#include "../header.h"

int main(int argc, char *argv[]) {
    std::string res_path = argv[1];
    std::string omop_map_path = argv[2];
    // Out File
    std::vector<std::string> full_res_path_vec = {res_path, "processed/data/", "all_minimal_omop.csv"};    
    std::string full_res_path = concat_string(full_res_path_vec, std::string(""));

    // Getting Concept data
    std::string line;
    std::ifstream omop_in;
    // These are multiple maps. Starts with source. Each source then has entries with the LAB IDs mapping to the OMOP IDs
    std::unordered_map<std::string, std::string> omop_map;
    std::unordered_map<std::string, std::string> omop_units;
    std::unordered_map<std::string, std::string> omop_names;

    std::unordered_map<std::string, std::unordered_map<std::string, std::string>> omop_abbreviations;
    omop_in.open(omop_map_path); check_in_open(omop_in, omop_map_path);
    while(std::getline(omop_in, line)) {
        std::vector<std::string> line_vec = split(line, "\t");
        std::string lab_id = line_vec[0];
        std::string source = line_vec[1];
        std::string abbreviation = line_vec[2];
        std::string unit = line_vec[3];
        std::string group_id = line_vec[4];
        std::string name = line_vec[5];
        // LAB_ID, SOURCE, ....., OMOP_ID
        omop_map[abbreviation] = group_id;
        omop_names[group_id] = name;
        omop_units[group_id] = unit;
    }
    omop_in.close();

    std::ofstream res_file;
    res_file.open(full_res_path); check_out_open(res_file, full_res_path); 
    // Counts
    int first_line = 1;


    // Lines
    std::string new_line;

    // In
    while(std::getline(std::cin, line)) {
        if(first_line == 1) {
            res_file << "FINREGISTRYID;DATE_TIME;SERVICE_PROVIDER;LAB_ID;LAB_ID_SOURCE;LAB_ABBREVIATION;LAB_VALUE;LAB_UNIT;LAB_ABNORMALITY;OMOP_ID;OMOP_NAME;OMOP_UNIT" << "\n";
            first_line = 0;
            continue;
        }
        // 0: FINREGISTRYID, 1: DATE, 2: LAB_NAME, 3: ID, 4: ID_SOURCE, 5: NAME, 6: ABBREVIATION, 7: VALUE, 8: UNIT, 9: ABNORMALITY
        std::vector<std::string> line_vec = split(line, ";");

        // Finding OMOP mapping
        std::string omop_id;
        std::string omop_name;
        std::string omop_unit;
        std::string lab_abbreviation = line_vec[5];

        omop_id = omop_map[lab_abbreviation];
        omop_name = omop_names[omop_id];
        omop_unit = omop_units[omop_id];


        // Writing to file
        res_file << line_vec[0] << ";" << line_vec[1] << ";" << line_vec[2] << ";" << line_vec[3] << ";" << line_vec[4] << ";" << lab_abbreviation << ";" << line_vec[6] << ";" << line_vec[7] << ";" << line_vec[8] << ";" << omop_id << ";" << omop_name << ";" << omop_unit << "\n";
    }

    res_file.close(); 
}