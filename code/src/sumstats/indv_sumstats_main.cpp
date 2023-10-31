#include "../header.h"


int main(int argc, char *argv[]) {
    std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();

    // Reading config file
    std::string config_path = argv[1];
    std::unordered_map<std::string, std::string> configs;
    read_config_file(config_path, configs);

    // Getting maps for selection of OMOPs and Individuals
    std::unordered_map<std::string, std::tuple<date, date>> relevant_indvs;
    read_indvs_date_file_from_config(configs, relevant_indvs, int(0)); // SampleFile in configs

    std::unordered_set<std::string> relevant_omops;
    read_omops_file_from_config(configs, relevant_omops, int(0));

    // Map for storing values of individuals for each OMOP
    // FINREGID -> OMOP_ID;LAB_UNIT -> VECTOR OF VALUES
    std::unordered_map<std::string, std::unordered_map<std::string, std::vector<double>>> indvs_omops_values;

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
        std::string lab_date_time = line_vec[1];
        std::string lab_service_provider = line_vec[2];
        std::string lab_id = remove_chars(line_vec[3], ' ');
        std::string lab_id_source = line_vec[4];
        std::string lab_abbrv = remove_chars(line_vec[5], ' ');
        std::string lab_value = remove_chars(line_vec[6], ' ');
        std::string lab_unit = remove_chars(line_vec[7], ' ');
        std::string lab_abnorm = remove_chars(line_vec[8], ' ');
        std::string omop_id = line_vec[9];
        std::string omop_name = line_vec[10];

        std::string omop_identifier = concat_string(std::vector<std::string>({omop_id, lab_unit}), std::string("_"));
        date lab_date(from_simple_string(lab_date_time));

        // Interested in this OMOP ID and unit combo
        if(relevant_omops.find(omop_identifier) != relevant_omops.end()) {
            // Interested in this individual
            if(relevant_indvs.find(finregid) != relevant_indvs.end()) {
                // Measurement date relevant for this individual
                date start_date = std::get<0>(relevant_indvs[finregid]);
                date end_date = std::get<1>(relevant_indvs[finregid]);
                if(lab_date >= start_date && lab_date <= end_date) {
                    // Add value to vector of omop values of individual
                    indvs_omops_values[finregid][omop_identifier].push_back(std::stod(lab_value));
                }
            // Interested in all data for all individuals
            } 
        } else if(relevant_indvs.empty()) {
                // Add value to vector of omop values of individual
                indvs_omops_values[finregid][omop_identifier].push_back(std::stod(lab_value));
        }

        // Write every 10000000 lines
        n_lines++;
        if(n_lines % 10000000 == 0) {
            std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();

            std::cout << "Lines read = " << n_lines << " Time took = " << std::chrono::duration_cast<std::chrono::minutes>(end - begin).count() << "[min]" << std::endl;
        }
    }

    // print first 5 entries in indvs_omops_values
    std::cout << "Printing first 5 entries in indvs_omops_values" << std::endl;
    int i = 0;
    for(auto const& indv_data : indvs_omops_values) {
        std::cout << indv_data.first << std::endl;
        int j = 0;
        for(auto const& indv_omop_data: indv_data.second) {
            std::cout << indv_omop_data.first << std::endl;
            int k = 0;
            for(auto const& omop_value: indv_omop_data.second) {
                std::cout << omop_value << std::endl;
                k++;
                if(k == 5) break;
            }
            j++;
            if(j == 5) break;
        }
        i++;
        if(i == 5) break;
    }

    std::cout << "Starting summary stats" << std::endl;
    // Writing mean, median, min, max, sd, first quantile, third quantile, n_elems
    write_indvs_omops_sumstats(indvs_omops_values, configs["ResPath"]);

    // close lab file
    lab_file.close();
    
    std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();

    std::cout << "Time took overall = " << std::chrono::duration_cast<std::chrono::hours> (end - begin).count() << "[h]" << std::endl;
}
