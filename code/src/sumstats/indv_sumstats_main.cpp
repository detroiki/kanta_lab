#include "../header.h"


int main(int argc, char *argv[]) {
    std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();

    std::string res_path = argv[1];
    std::string omops_path = argv[2];
    std::string indvs_path = argv[3];

    // Getting omop ids
   std::unordered_set<std::string> relevant_omops;
   std::unordered_map<std::string, std::tuple<date, date>> relevant_indvs;
   read_omops_file(relevant_omops, omops_path);
   read_indvs_files(relevant_indvs, indvs_path);

   std::unordered_map<std::string, std::unordered_map<std::string, std::vector<double>>> indvs_omops_values;

    // Reading
    std::string line;
    int first_line = 1; // Indicates header line
    int n_lines = 0;
    while(std::getline(std::cin, line)) {
        if (first_line == 1) {
            first_line = 0;
            continue;
        }

        std::vector<std::string> line_vec(split(line, ";"));
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

        std::string omop_identifier = concat_string({omop_id, lab_unit}, std::string(";"));
        date lab_date(from_simple_string(lab_date_time));

        // Interested in this OMOP ID and unit combo
        if((relevant_omops.find(omop_identifier) != relevant_omops.end()) &
            // Interested in this individual
            (relevant_indvs.find(finregid) != relevant_indvs.end())) {
                // Measurement date relevant for this individual
                date start_date = std::get<0>(relevant_indvs[finregid]);
                date end_date = std::get<1>(relevant_indvs[finregid]);
                if(lab_date >= start_date && lab_date <= end_date) {
                    // Add value to vector of omop values of individual
                    indvs_omops_values[finregid][omop_identifier].push_back(std::stod(lab_value));
                }
        }

        // Write every 100000 lines
        n_lines++;
        if(n_lines % 10000000 == 0) {
            std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();

            std::cout << "Lines read = " << n_lines << " Time took = " << std::chrono::duration_cast<std::chrono::minutes>(end - begin).count() << "[min]" << std::endl;
        }
    }

    std::cout << "Starting summery stats" << std::endl;
    // Writing mean, median, min, max, sd, first quantile, third quantile, n_elems
    write_indvs_omops_sumstats(indvs_omops_values, res_path);

    std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();

    std::cout << "Time took overall = " << std::chrono::duration_cast<std::chrono::hours> (end - begin).count() << "[h]" << std::endl;
}
