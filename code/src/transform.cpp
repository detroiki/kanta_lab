#include "header.h"

/**
 * @brief Performs final fixes to the data
 * 
 * @param argc The number of arguments
 * @param argv The arguments. The first argument is the path to the results folder.
 * 
 * @return int
 * 
 * This program performs final fixes to the data. It is run after the data has been processed
 * and the OMOP IDs have been added. The fixes are:
 * - Fixing percentages that are in osuus (fraction) format into % format
 * - Fixing abnormality abbreviations to be consistent. This means replacing < with L, > with H, 
 *    POS with A and NEG with N. If the abbreviation is not one of these, it is replaced with NA.
 * - Removing lines where the measurement year is before 2014
 * - Removing lines where the lab value is not a number. Makes illegal units that are numbers NA. 
 * - Removing values from title lines (Making them NAs) and turning the lab unit to ordered. 
 *   These are lines where often there is random information in the lab value column. 
 * - Moving lab abnormality information to the lab value column if the lab value is NA. These are 
 *   marked with binary in the lab unit column.
*/
int main(int argc, char *argv[]) {
    std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();

    std::string full_res_path = argv[1];
    std::string direction = argv[2];

    // Opening results file
    std::ofstream res_file;
    res_file.open(full_res_path);
    check_out_open(res_file, full_res_path);

    // Reading
    if(direction == ",->\t") {
        char in_delim = ',';
        char out_delim = '\t';

        std::string line;
        int n_lines = 0;
        while(std::getline(std::cin, line)) {
            std::vector<std::string> line_vec = splitString(line, in_delim);
            for(unsigned int i = 0; i < line_vec.size(); i++) {
                line_vec[i] = remove_chars(line_vec[i], '\t');
            }
            res_file << concat_string(line_vec, std::string(1, out_delim)) << "\n";
            n_lines++; write_line_update(n_lines, begin);
        }
    } else if(direction == "\t->,") {
        char in_delim = '\t';
        char out_delim = ',';

        std::string line;
        int n_lines = 0;
        while(std::getline(std::cin, line)) {
            std::vector<std::string> line_vec = split(line, &in_delim);
            for(unsigned int i = 0; i < line_vec.size(); i++) {
                add_quotation(line_vec[i], out_delim);
            }
            res_file << concat_string(line_vec, std::string(1, out_delim)) << "\n";
            n_lines++; write_line_update(n_lines, begin);
        }
    }

}

