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
    std::unordered_map<std::string, std::unordered_map<std::string, std::string>> omop_map;
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
        omop_map[source][lab_id] = group_id;
        omop_names[group_id] = name;
        omop_units[lab_id] = unit;
        omop_abbreviations[source][lab_id] = abbreviation;
    }
    omop_in.close();

    std::ofstream res_file;
    res_file.open(full_res_path); check_out_open(res_file, full_res_path); 
    // Counts
    unsigned long long total_line_count = 0;


    // Lines
    std::string new_line;

    // In
    while(std::getline(std::cin, line)) {
        if(total_line_count == 0) {
            res_file << "FINREGISTRYID;DATE_TIME;service_provider_name;LAB_ID;LAB_ABBREVIATION;LAB_VALUE;LAB_UNIT;LAB_ABNORMALITY;OMOP_ID;OMOP_NAME;OMOP_ABBREVIATION;OMOP_UNIT" << "\n";
            total_line_count++;
            continue;
        }
        total_line_count++;
        // 0: FINREGISTRYID, 1: DATE, 2: LAB_NAME, 3: ID, 4: ID_SOURCE, 5: NAME, 6: ABBREVIATION, 7: VALUE, 8: UNIT, 9: ABNORMALITY
        std::vector<std::string> line_vec = split(line, ";");

        // Getting current lab source
        std::string lab_source;
        if(line_vec[2].find("Helsinki") != std::string::npos) {
            lab_source = "LABfi_HUS";
        } else if(line_vec[2].find("Tampere") != std::string::npos) {
            lab_source = "LABfi_TMP";
        } else if(line_vec[2].find("Turku") != std::string::npos) {
            lab_source = "LABfi_TKU";
        } else {
            lab_source = "LABfi";
        }

        // Finding OMOP ID
        std::string omop_id;
        std::string omop_name;
        std::string omop_unit;
        std::string omop_abbreviation;
        std::string lab_id = line_vec[3];
        if(omop_map[lab_source].find(lab_id) != omop_map[lab_source].end()) {
            omop_id = omop_map[lab_source][lab_id];
            omop_name = omop_names[omop_id];
            omop_unit = omop_units[lab_id];
            omop_abbreviation = omop_abbreviations[lab_source][lab_id];

        } else if(omop_map["LABfi"].find(lab_id) != omop_map["LABfi"].end()) {
            omop_id = omop_map["LABfi"][lab_id];
            omop_name = omop_names[omop_id];
            omop_unit = omop_units[lab_id];
            omop_abbreviation = omop_abbreviations["LABfi"][lab_id];
        } else {
            omop_id = "NA"; omop_name = "NA"; omop_unit = "NA"; omop_abbreviation = "NA";
        }

        std::string lab_abbreviation;
        if(line_vec[4] == "0") {
            lab_abbreviation = line_vec[5];
        } else {
            lab_abbreviation = line_vec[6];
        }
        // Writing to file
        res_file << line_vec[0] << ";" << line_vec[1] << ";" << line_vec[2] << ";" << lab_id << ";" << lab_abbreviation << ";" << line_vec[7] << ";" << line_vec[8] << ";" << line_vec[9] << ";" << omop_id << ";" << omop_name << ";" << omop_abbreviation << ";" << omop_unit << "\n";
    }

    res_file.close(); 
}