#include "../header.h"


int main(int argc, char *argv[]) {
    std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();

    std::string res_path = argv[1];

    // Opening results file
    std::vector<std::string> full_res_path_vec = {res_path, "processed/reports/omop_maps/omop_sumstats.csv"};
    std::string full_res_path = concat_string(full_res_path_vec);
    std::ofstream res_file;
    res_file.open(full_res_path);
    check_out_open(res_file, full_res_path);

    // Reading in maps for new omop concepts
    std::unordered_map<std::string, std::vector<double>> omops;

    // Reading
    std::string line;
    int first_line = 1; // Indicates header line
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

        if(!(omop_id == "NA" || omop_name == "NA" || lab_value == "NA" || lab_unit == "binary")) {
            std::string omop_identifier = get_omop_identifier(omop_id, omop_name, lab_unit, std::string(";"));
            omops[omop_identifier].push_back(std::stod(lab_value));
        }
    }

    write_omop_sumstats(omops, res_file);  
    res_file.close();

    std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();

    std::cout << "Time took = " << std::chrono::duration_cast<std::chrono::minutes>(end - begin).count() << "[min]" << std::endl;
    std::cout << "Time took = " << std::chrono::duration_cast<std::chrono::seconds> (end - begin).count() << "[s]" << std::endl;
}
