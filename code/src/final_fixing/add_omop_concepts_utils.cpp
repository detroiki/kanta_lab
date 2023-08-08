#include "../header.h"

/**
 * @brief Gets new OMOP mappings from file
 * 
 * @param new_omops The map to store the new OMOP mappings
 * @param file_path The path to the file containing the new OMOP mappings
 * 
 * @return void
 * 
 * These mappings are based on the lab ID, abbreviation, and unit.
*/
void get_new_omop_concepts(std::unordered_map<std::string, std::string> &new_omops,
                           std::unordered_map<std::string, std::string> &omop_names,   
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
        std::vector<std::string> line_vec = split(line, ";");

        std::string omop_id = line_vec[0];
        std::string lab_abbreviation = line_vec[2];
        std::string lab_id = line_vec[3];
        std::string lab_unit = line_vec[4];

        std::string omop_identifier = get_omop_identifier(lab_id, lab_abbreviation, lab_unit);
        new_omops[omop_identifier] = omop_id;

        std::string omop_name = line_vec[1];
        omop_names[omop_id] = omop_name;
    }   

    in_file.close(); 
}
