#include "../header.h"

int main(int argc, char *argv[]) {
    std::string new_omops_path = argv[1];
    std::string res_path = argv[2];

    // Opening results file
    std::vector<std::string> full_res_path_vec = {res_path, "processed/data/kanta_lab_minimal_omop_new.csv"};
    std::string full_res_path = concat_string(full_res_path_vec);
    std::ofstream res_file;
    res_file.open(full_res_path);
    check_out_open(res_file, full_res_path);

    // Reading in maps for new omop concepts
    std::unordered_map<std::string, std::string> new_omops;
    std::unordered_map<std::string, std::string> omop_names;
    get_new_omop_concepts(new_omops, omop_names, new_omops_path);


    // Reading
    std::string line;
    int first_line = 1; // Indicates header line
    while(std::getline(std::cin, line)) {
        if (first_line == 1) {
            res_file << line << "\n";
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

        std::string omop_identifier = get_omop_identifier(lab_id, lab_abbrv, lab_unit);
        if(new_omops.find(omop_identifier) != new_omops.end()) {
            omop_id = new_omops[omop_identifier];
            omop_name = omop_names[omop_id];
        }

        res_file << finregid << ";" << lab_date_time << ";" << lab_service_provider << ";" << lab_id << ";" << lab_id_source << ";" << lab_abbrv << ";" << lab_value << ";" << lab_unit << ";" << lab_abnorm << ";" << omop_id << ";" << omop_name << "\n";

    }
    res_file.close();
}
