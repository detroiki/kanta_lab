#include "../header.h"


int main(int argc, char *argv[]) {
    std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();

    // Reading config file
    std::string config_path = argv[1];
    std::unordered_map<std::string, std::string> configs;
    read_config_file(config_path, configs);

    // Relevant OMOPs
    std::unordered_set<std::string> relevant_omops;
    read_omops_file_from_config(configs, relevant_omops, int(0));

    // Map to all lab ID / abbreviations
    std::unordered_map<std::string, std::unordered_set<std::string>> omop_lab_map;
    std::unordered_map<std::string, std::unordered_set<std::string>> lab_indvs;
    std::unordered_map<std::string, int> lab_counts;

    // Open "KantaLabFile"
    std::ifstream lab_file;
    lab_file.open(configs["KantaLabFile"]);
    check_in_open(lab_file, configs["KantaLabFile"]);

    // Reading
    std::string line;
    int first_line = 1; // Indicates header line
    int n_lines = 0;
    while(std::getline(lab_file, line)) {
        if (first_line == 1) {
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

        std::string omop_identifier = concat_string(std::vector<std::string>({omop_id, lab_unit}), std::string("_"));
        std::string lab_identifier = concat_string(std::vector<std::string>({lab_id, lab_abbrv}), std::string("_"));

        if(relevant_omops.find(omop_identifier) != relevant_omops.end() || relevant_omops.empty()) {
            omop_lab_map[omop_identifier].insert(lab_identifier);
            lab_indvs[lab_identifier].insert(finregid);
            lab_counts[lab_identifier]++;
        }

        // Write every 10000000 lines
        n_lines++;
        if(n_lines % 10000000 == 0) {
            std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();

            std::cout << "Lines read = " << n_lines << " Time took = " << std::chrono::duration_cast<std::chrono::minutes>(end - begin).count() << "[min]" << std::endl;
        }
    }

    // Write map to file
    std::string omop_lab_map_file_path = configs["ResPath"] + std::string("_omop_lab_map.csv");
    std::ofstream omop_lab_map_file;
    omop_lab_map_file.open(omop_lab_map_file_path);
    check_out_open(omop_lab_map_file, omop_lab_map_file_path);

    // Write header
    omop_lab_map_file << "OMOP_IDENTIFIER;LAB_IDENTIFIER;LAB_COUNT;INDV_COUNT" << std::endl;
    for(auto const& omop_identifier_data : omop_lab_map) {
        std::string omop_identifier = omop_identifier_data.first;
        for(auto const& lab_identifier : omop_identifier_data.second) {
            omop_lab_map_file << omop_identifier << ";" << lab_identifier << ";" << lab_counts[lab_identifier] << ";" << lab_indvs[lab_identifier].size() << std::endl;
        }
    }

    // close lab file
    omop_lab_map_file.close();
    
    write_end_run_summary(begin);
}
