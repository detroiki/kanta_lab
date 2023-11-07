
#include "../header.h"


/**
 * @brief Main function for adding OMOP concepts to lab data
 * 
 * @param argc Number of command line arguments
 * @param argv Command line arguments
 * 
 * @return int
 * 
 * Command line arguments:
 * 1. Path to file containing new OMOP mappings
 * 2. Path to results directory
 * 3. Minimum count for new OMOP mappings - so how often this lab ID appears together with the abbreviation and unit that are mapped to the OMOP concept
 * 
 * Reads in lab data from stdin and adds OMOP concepts to it.
*/
int main(int argc, char *argv[]) {
    std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();

    std::string new_omops_path = argv[1];
    std::string res_path = argv[2];
    std::string date = argv[3];
    // Read min counts input from command line
    int min_count = std::stoi(argv[4]);

    // Opening results file
    std::vector<std::string> full_res_path_vec = {res_path, "processed/data/kanta_lab_", date, "_omop_new.csv"};
    std::string full_res_path = concat_string(full_res_path_vec);
    std::ofstream res_file;
    res_file.open(full_res_path);
    check_out_open(res_file, full_res_path);

    // Reading in maps for new omop concepts
    std::unordered_map<std::string, std::string> new_omops;
    std::unordered_map<std::string, std::string> omop_names;
    get_new_omop_concepts(new_omops, omop_names, new_omops_path, min_count);


    // Reading
    std::string line;
    int first_line = 1; // Indicates header line
    int n_lines = 0;
    while(std::getline(std::cin, line)) {
        if (first_line == 1) {
            res_file << line << "\n";
            first_line = 0;
            continue;
        }

        std::vector<std::string> line_vec(splitString(line, ','));
        std::string finregid = line_vec[0];
        std::string date_time = line_vec[1];
        std::string service_provider = line_vec[2];
        std::string lab_id = remove_chars(line_vec[3], ' ');
        std::string lab_id_source = line_vec[4];
        std::string lab_abbrv = remove_chars(line_vec[5], ' ');
        std::string lab_value = remove_chars(line_vec[6], ' ');
        std::string lab_unit = remove_chars(line_vec[7], ' ');
        std::string lab_abnorm = remove_chars(line_vec[8], ' ');
        std::string omop_id = line_vec[9];
        std::string omop_name = line_vec[10];

        std::string omop_identifier = get_omop_identifier(lab_id, lab_abbrv, lab_unit, std::string(","));
        if(new_omops.find(omop_identifier) != new_omops.end()) {
            omop_id = new_omops[omop_identifier];
            omop_name = omop_names[omop_id];
        }
        
        add_quotation(lab_id); add_quotation(lab_value); add_quotation(lab_abbrv); add_quotation(lab_unit); add_quotation(omop_name);

        // Writing to results file

        res_file << finregid  << "," << date_time << "," <<service_provider << "," << lab_id << "," << lab_id_source << "," << lab_abbrv << "," << lab_value << "," <<lab_unit << "," << lab_abnorm << "," <<omop_id << "," << omop_name << "\n";     

        // Write every 10000000 lines
        n_lines++; write_line_update(n_lines, begin);
    }
    res_file.close();

    write_end_run_summary(begin);
}


