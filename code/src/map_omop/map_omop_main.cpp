#include "../header.h"

/**
 * @brief Maps OMOP Concept IDs and names to the minimal data
 * 
 * @param argc The number of arguments
 * @param argv The arguments
 * 
 * @return void
 * 
 * Maps the OMOP concetps IDs, using both the lab ID and abbreviation map. 
 * We can map about 87% of the data this way
 * 
 * Reads in the minimal data from stdin. The delimiter is expected to be ";"
 * Expects the columns to be:
 * - FINREGISTRYID
 * - LAB_DATE_TIME
 * - LAB_SERVICE_PROVIDER
 * - LAB_ID
 * - LAB_ID_SOURCE
 * - LAB_ABBREVIATION
 * - LAB_VALUE
 * - LAB_UNIT
 * - LAB_ABNORMALITY
 * The column names are irrelevant but they need to be in the correct order.
 * 
 * The lab ID and abbreviations are mapped to the correct source table, if possible and 
 * otherwise hierarchical to the best matching source table. The four tables are
 * LABfi the nation wide table, LABfi_HUS the HUS table, LABfi_TMP the Tampere table, and
 * LABfi_TKU the Turku table. The hierarchy for local lab IDs from non-major hospitals is 
 * HUS, TMP, TKU.
 * 
 * Expects the following commandline arguments:
 *  - res_path: The path to the results directory
 *  - omop_concept_map_path: The path to the OMOP concept ID map. 
 *      Mapping from lab IDs and abbreviations to OMOP concept IDs. The delimiter is expected
 *      to be "\t". Expects columns: LAB_ID, LAB_SOURCE, LAB_ABBREVIATION, UNIT, OMOP_ID, NAME. 
 *      The columns names are irrelevant but they need to be in the correct order. 
 *      LAB_SOURCE is either LABfi, LABfi_HUS, LABfi_TMP, LABfi_TKU.
**/
int main(int argc, char *argv[]) {
    std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();

    // Arguments
    std::string res_path = argv[1];
    std::string omop_concept_map_path = argv[2];
    std::string date = argv[3];

    // Results File
    std::vector<std::string> full_res_path_vec = {res_path, "processed/data/kanta_lab_", date, "_omop.csv"};    
    std::string full_res_path = concat_string(full_res_path_vec, std::string(""));
    std::ofstream res_file;
    res_file.open(full_res_path); check_out_open(res_file, full_res_path); 

    // OMOP Maps
    // The OMOP concept ID has separate maps for each lab source
    // LABfi, LABfi_HUS, LABfi_TMP, LABfi_TKU
    std::unordered_map<std::string, std::unordered_map<std::string, std::string>> omop_concept_map;
    // The mapping to the OMOP name is unique for each group ID
    std::unordered_map<std::string, std::string> omop_names;
    // Reading in OMOP map from file
    read_omop_file(omop_concept_map_path, omop_concept_map, omop_names);

    // Flag for first line
    int first_line = 1;
    int n_lines = 0;

    // Reading
    std::string line;
    while(std::getline(std::cin, line)) {
        if(first_line == 1) {
            // Column headers
            res_file << "FINREGISTRYID,LAB_DATE_TIME,LAB_SERVICE_PROVIDER,LAB_ID,LAB_ID_SOURCE,LAB_ABBREVIATION,LAB_VALUE,LAB_UNIT,LAB_ABNORMALITY,OMOP_ID,OMOP_NAME" << "\n";
            first_line = 0;
            continue;
        }

        // Splitting line
        std::vector<std::string> line_vec = splitString(line, ',');
        std::string finregid = line_vec[0];
        std::string date_time = line_vec[1];
        std::string service_provider = line_vec[2];
        std::string lab_id = remove_chars(line_vec[3], ' ');
        std::string lab_id_source = line_vec[4];
        std::string lab_abbrv = remove_chars(line_vec[5], ' ');
        std::string lab_value = line_vec[6];
        std::string lab_unit = line_vec[7];
        std::string lab_abnormality = line_vec[8];  

        // Getting current lab source (LABfi, LABfi_HUS, LABfi_TMP, LABfi_TKU)
        std::string omop_lab_source = get_omop_lab_source(lab_id_source, service_provider);
  
        // Finding OMOP mapping
        // Currently identifying the OMOP concept by the lab ID and abbreviation.
        // We can map about 87% of the data this way
        std::string lab_id_abbrv = get_lab_id_abbrv(lab_id, lab_abbrv);
        // also print only the first ten elements in omop_concept_map
        std::string omop_id = get_omop_id(omop_concept_map, omop_lab_source, lab_id_abbrv);
        std::string omop_name = get_omop_name(omop_id, omop_names);

        // Writing to results file
        add_quotation(lab_id); add_quotation(lab_value); add_quotation(lab_abbrv); add_quotation(lab_unit); add_quotation(omop_name);

        res_file << finregid  << "," << date_time << "," <<service_provider << "," << lab_id << "," << lab_id_source << "," << lab_abbrv << "," <<  lab_value << "," << lab_unit << "," << lab_abnormality << "," <<omop_id << "," << omop_name << "\n";

        // Write every 10000000 lines
        n_lines++;
        if(n_lines % 10000000 == 0) {
            std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();

            std::cout << "Lines read = " << n_lines << " Time took = " << std::chrono::duration_cast<std::chrono::minutes>(end - begin).count() << "[min]" << std::endl;
        }
    }

    res_file.close(); 

    std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();

    std::cout << "Time took = " << std::chrono::duration_cast<std::chrono::minutes>(end - begin).count() << "[minutes]" << std::endl;
}
