#include "../header.h"

/**
 * @brief Creates a map from the OMOP IDs to every individual ID for which there is data
 * 
 * @param lab_indv_count The map of OMOP IDs to the individuals ID for which there is data
 * @param file_path The path to the file to read
 * 
 * @return void
 * 
 * Adds information to the map lab_indv_count. The map is of the form:  
 * OMOP_ID -> {FINREGISTRYID1, FINREGISTRYID2, ...}.
*/
void get_lab_indv_counts(std::unordered_map<std::string, std::unordered_set<std::string>> &lab_indv_count,
                         std::string file_path) {
    // Opening file
    std::ifstream in_file;
    in_file.open(file_path); check_in_open(in_file, file_path); 

    // Reading
    std::string line;
    int first_line = 1; // Indicates header line
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

/**
 * @brief Gets the OMOP IDs that have data for more than 5% of the individuals
 * 
 * @param keep_omop_ids The set of OMOP IDs to keep
 * @param lab_indv_count The map of OMOP IDs to the individuals ID for which there is data
 * 
 * @return void
 * 
 * Gets the OMOP IDs that have data for more than 5% of the individuals and adds them to the
 * set keep_omop_ids.
*/
void get_keep_omop_ids(std::unordered_set<std::string> &keep_omop_ids, 
                       std::unordered_map<std::string, std::unordered_set<std::string>> &lab_indv_count) {
    for(auto omop_id: lab_indv_count) {
        // Min number of individuals (5% of alive in 2021)
        if(omop_id.second.size() > 281008){
            keep_omop_ids.insert(omop_id.first);
        }
    }
}


/**
 * @brief Writes the top lab data to file based on the OMOP IDs with data for more than 5% of the individuals
 * 
 * @param file_path The path to the file to read
 * @param res_path The path to the results folder
 * @param keep_omop_ids The set of OMOP IDs to keep
 * 
 * @return void
*/
void write_top_lab_data(std::string file_path,
                        std::string res_path, 
                        std::unordered_set<std::string> keep_omop_ids) {
    /// Opening
    // In file
    std::ifstream in_file;
    in_file.open(file_path); check_in_open(in_file, file_path); 
    // Out file
    std::vector<std::string> full_res_path_vec = {res_path, "processed/data/kanta_lab_minimal_omop_top.csv"};    
    std::string full_res_path = concat_string(full_res_path_vec, std::string(""));
    std::ofstream res_file;
    res_file.open(full_res_path); check_out_open(res_file, full_res_path); 

    // Reading
    std::string line;
    int first_line = 1; // Indicates header line
    while(std::getline(in_file, line)) {
        if(first_line == 1) {
            res_file << line << "\n";
            first_line = 0;
            continue;
        }

        std::vector<std::string> line_vec = split(line, ";");
        std::string omop_id = line_vec[9];

        // Writing to file if OMOP ID is in keep set
        if(keep_omop_ids.find(omop_id) != keep_omop_ids.end()) {
            res_file << line << "\n";
        }
    }
    in_file.close();
    res_file.close();
}