#include "../header.h"

void read_finreg_map(std::string finreg_path,
                     std::unordered_map<std::string, std::string> &finreg_map) {
    // Opening file
    std::ifstream in_finreg;
    in_finreg.open(finreg_path); check_in_open(in_finreg, finreg_path);

    // Reading
    std::string line;
    int first_line = 1; // Indicates header line
    while(std::getline(in_finreg, line)) {
        if(first_line == 1) {
            first_line = 0;
            continue;
        }
        std::vector<std::string> line_vec(split(line, ","));   
        std::string finregid = line_vec[0];
        std::string yob = line_vec[2];
        std::string sex = line_vec[3];

        std::string yob_sex = concat_string({yob, sex}, ";");
        finreg_map[0] = yob_sex;
    }
}

/**
 * @brief Writes the updated file with individuals sex and year of birth from the finregistry file
 * 
 * @param file_path The path to the file to read
 * @param res_path The path to the results folder
 * 
*/
void write_updated_file(std::string res_path,
                        std::unordered_map<std::string, std::string> &finreg_map) {
    // Opening results file
    std::vector<std::string> full_res_path_vec = {res_path, "processed/data/kanta_lab_minimal_omop_sex_yob.csv"};
    std::string full_res_path = concat_string(full_res_path_vec);
    std::ofstream res_file;
    res_file.open(full_res_path); check_out_open(res_file, full_res_path);

    // Reading
    std::string line;
    int first_line = 1; // Indicates header line
    while(std::getline(std::cin, line)) {
        if(first_line == 1) {
            res_file << "FINREGISTRYID;DATE_TIME;SERVICE_PROVIDER;LAB_ID;LAB_ID_SOURCE;LAB_ABBREVIATION;LAB_VALUE;LAB_UNIT;LAB_ABNORMALITY;OMOP_ID;OMOP_NAME;SEX;YEAR_OF_BIRTH" << "\n";
            first_line = 0;
            continue;
        }

        std::vector<std::string> line_vec(split(line, ";"));
        std::string finregistryid = line_vec[0];

        res_file << line << ";" << finreg_map[finregistryid] << "\n";
    }
    res_file.close();
}