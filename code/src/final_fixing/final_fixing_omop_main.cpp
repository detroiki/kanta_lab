#include "../header.h"

int main(int argc, char *argv[]) {
    std::string file_path = argv[1];
    std::string res_path = argv[2];

    // Opening results file
    std::vector<std::string> full_res_path_vec = {res_path, "processed/data/kanta_lab_minimal_omop_top_final.csv"};
    std::string full_res_path = concat_string(full_res_path_vec);
    std::ofstream res_file;
    res_file.open(full_res_path);
    check_out_open(res_file, full_res_path);

    // Problem rows file
    std::vector<std::string> error_path_vec = {res_path, "processed/reports/problem_rows/removed_omop_top_final_rows.csv"};
    std::string error_path = concat_string(error_path_vec, std::string(""));
    std::ofstream error_file;
    error_file.open(error_path);
    check_out_open(error_file, error_path);


    // Counting number of individuals each OMOP concept ID has
    std::unordered_map<std::string, std::string> omop_max_units;
    get_omop_max_units(omop_max_units, file_path);

    // Opening file
    std::ifstream in_file;
    in_file.open(file_path); check_in_open(in_file, file_path); 

    // Reading
    std::string line;
    int first_line = 1; // Indicates header line
    while(std::getline(in_file, line)) {
        if (first_line == 1) {
            res_file << "FINREGISTRYID;LAB_DATE_TIME;LAB_SERVICE_PROVIDER;LAB_ID;LAB_ID_SOURCE;LAB_ABBREVIATION;LAB_VALUE;LAB_UNIT;LAB_ABNORMALITY;OMOP_ID;OMOP_NAME;SEX;YEAR_OF_BIRTH"
                     << "\n";
            error_file << "FINREGISTRYID;LAB_DATE_TIME;LAB_SERVICE_PROVIDER;LAB_ID;LAB_ID_SOURCE;LAB_ABBREVIATION;LAB_VALUE;LAB_UNIT;LAB_ABNORMALITY;OMOP_ID;OMOP_NAME;SEX;YEAR_OF_BIRTH"
                       << "\n";
            first_line = 0;
            continue;
        }

        std::vector<std::string> line_vec(split(line, ";"));
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
        std::string sex = line_vec[11];
        std::string yob = line_vec[12];


        // Fixing values
        fix_phs(lab_id, lab_abbrv, lab_unit);  // Phs often have no units

        // Seeing if there are lines to be fully removed from the data
        int keep = decide_keep_rows(omop_id, lab_unit, omop_max_units);
        if (keep == 1) {
            res_file << finregid << ";" << date_time << ";" << service_provider << ";" << lab_id << ";" << lab_id_source << ";" << lab_abbrv << ";" << lab_value << ";" << lab_unit << ";" << lab_abnorm << ";" << omop_id << ";" << omop_name << ";" << sex << ";" << yob << "\n";
        } else {
            error_file << line << "\n";
        }
    }
    error_file.close();
    res_file.close();
}
