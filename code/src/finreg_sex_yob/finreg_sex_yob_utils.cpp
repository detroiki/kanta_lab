#include "../header.h"

void read_finreg_map(std::string finreg_path,
                     std::unordered_map<std::string, std::string> &finreg_map) {
    cout << "Reading FinRegistry file" << endl;
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
        std::string finregid = remove_chars(line_vec[0], '\"');

        std::string date_birth = remove_chars(line_vec[2], '\"');
        std::string year_birth = date_birth.substr(6, 4);
        std::string month_birth = date_birth.substr(3, 2);
        std::string day_birth = date_birth.substr(0, 2);
        std::vector<std::string> date_birth_vec = {year_birth, month_birth, day_birth};
        date_birth = concat_string(date_birth_vec, std::string("-"));

        std::string sex = line_vec[3];

        std::vector<std::string> sex_yob_vec = {sex, date_birth};
        std::string sex_yob = concat_string(sex_yob_vec, std::string(";"));

        finreg_map[finregid] = sex_yob;
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
    cout << "Writing information to file" << endl;

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
            res_file << "FINREGISTRYID;LAB_DATE_TIME;LAB_SERVICE_PROVIDER;LAB_ID;LAB_ID_SOURCE;LAB_ABBREVIATION;LAB_VALUE;LAB_UNIT;LAB_ABNORMALITY;OMOP_ID;OMOP_NAME;SEX;YEAR_OF_BIRTH" << "\n";
            first_line = 0;
            continue;
        }

        std::vector<std::string> line_vec(split(line, ";"));
        std::string finregid = line_vec[0];
        std::string sex_yob;
        if(finreg_map.find(finregid) != finreg_map.end()) {
            sex_yob = finreg_map[finregid];
        } else {
            sex_yob = "NA;NA";
        }
        res_file << line << ";" << sex_yob << "\n";
    }
    res_file.close();
}