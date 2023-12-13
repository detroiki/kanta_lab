#include "../header.h"

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
int main(int argc, char *argv[])
{
    std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();

    std::string res_path = argv[1];
    std::string date = argv[2];

    // Opening results file
    std::vector<std::string> full_res_path_vec = {res_path, "processed/data/kanta_lab_", date, "_final_fix.csv"};
    std::string full_res_path = concat_string(full_res_path_vec);
    std::ofstream res_file;
    res_file.open(full_res_path);
    check_out_open(res_file, full_res_path);

    // Problem rows file
    std::vector<std::string> error_path_vec = {res_path, "processed/reports/problem_rows/removed_final_rows.csv"};
    std::string error_path = concat_string(error_path_vec, std::string(""));
    std::ofstream error_file;
    error_file.open(error_path);
    check_out_open(error_file, error_path);

    // Reading
    char delim = ',';
    std::string line;
    int first_line = 1; // Indicates header line
    int n_lines = 0;
    while (std::getline(std::cin, line))
    {
        if (first_line == 1)
        if(first_line == 1) {
            // Column headers
            res_file << "FINREGISTRYID,LAB_DATE_TIME,LAB_SERVICE_PROVIDER,LAB_ID,LAB_ID_SOURCE,LAB_ABBREVIATION,LAB_VALUE,LAB_UNIT,OMOP_ID,OMOP_NAME,LAB_ABNORMALITY,REFERENCE_VALUE_TEXT,DATA_SYSTEM,DATA_SYSTEM_VERSION\n"; 
            error_file << "FINREGISTRYID,LAB_DATE_TIME,LAB_SERVICE_PROVIDER,LAB_ID,LAB_ID_SOURCE,LAB_ABBREVIATION,LAB_VALUE,LAB_UNIT,OMOP_ID,OMOP_NAME,LAB_ABNORMALITY,REFERENCE_VALUE_TEXT,DATA_SYSTEM,DATA_SYSTEM_VERSION\n"; 

            first_line = 0;
            continue;
        }

        std::vector<std::string> line_vec(splitString(line, delim));
        std::string finregid = line_vec[0];
        std::string lab_date_time = line_vec[1];
        std::string service_provider_name = line_vec[2];
        std::string lab_id = remove_chars(line_vec[3], ' ');
        std::string lab_id_source = line_vec[4];
        std::string lab_abbrv = remove_chars(line_vec[5], ' ');
        std::string lab_value = remove_chars(line_vec[6], ' ');
        std::string lab_unit = remove_chars(line_vec[7], ' ');
        std::string omop_id = line_vec[8];
        std::string omop_name = line_vec[9];
        std::string lab_abnormality = remove_chars(line_vec[10], ' ');
        std::string ref_value_text = line_vec[11];
        std::string data_system = line_vec[12];
        std::string data_system_ver = line_vec[13];

        // Fixing values and units
        fix_abnorms(lab_abnormality);
        remove_illegal_units(lab_unit);
        fix_phs(lab_id, lab_abbrv, lab_unit);  // Phs often have no units
        fix_inrs(lab_id, lab_abbrv, lab_unit); // INRs often have no units
        fix_titles(lab_id, lab_abbrv, lab_unit, lab_value); // Titles often have random units and values even though they are not measurements

        // Seeing if there are lines to be fully removed from the data
        int keep = 1;
        keep = remove_illegal_measure_year(lab_date_time, keep);
        keep = fix_percentages(lab_value, lab_unit, lab_abnormality, keep);
        keep = remove_illegal_values(lab_value, lab_abnormality, lab_abbrv, keep);

        shuffle_lab_abnorm_info(lab_value, lab_abnormality, lab_unit);
        
        // Writing line to file
        std::vector<std::string> final_line_vec = {finregid, lab_date_time, service_provider_name, lab_id, lab_id_source, lab_abbrv, lab_value, lab_unit, omop_id, omop_name, lab_abnormality, ref_value_text, data_system, data_system_ver};
        // Making sure that all columns with the delimiter in the text are in quotation marks
        for(int i = 0; i < final_line_vec.size(); ++i) add_quotation(final_line_vec[i], delim);
        res_file << concat_string(final_line_vec, std::string(1, delim)) << "\n";

        // Write every 10000000 lines
        n_lines++; write_line_update(n_lines, begin);
    }
    error_file.close();
    res_file.close();

    write_end_run_summary(begin);
}

