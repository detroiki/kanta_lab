#include "../header.h"

/**
 * @brief Creates a map from the OMOP IDs to the number of times each lab unit appears for that ID
 * 
 * @param omop_unit_count The map of OMOP IDs to the number of times each lab unit appears for that ID
 * @param file_path The path to the file to read
 * 
 * @return void
 * 
 * Adds information to the map omop_unit_count. The map is of the form:
 * OMOP_ID -> {lab_unit1: count1, lab_unit2: count2, ...}.
*/
void get_omop_unit_counts(std::unordered_map<std::string, std::unordered_map<std::string, int>> &omop_unit_count,
                          std::string file_path) {
    // Opening file
    std::ifstream in_file;
    in_file.open(file_path); check_in_open(in_file, file_path); 

    // Reading
    std::string line;
    int first_line = 1; // Indicates header line
    while(std::getline(in_file, line)) {
        if(first_line == 1) {
            first_line = 0;
            continue;
        }
        std::vector<std::string> line_vec = split(line, ";");

        std::string finregid = line_vec[0];
        std::string omop_id = line_vec[9];
        std::string lab_unit = line_vec[7];

        if(omop_id != "NA") {
            omop_unit_count[omop_id][lab_unit] += 1;
        }
    }

    in_file.close(); 
}

void get_omop_max_units(std::unordered_map<std::string, std::string> &omop_max_units,
                        std::string file_path) {
    std::unordered_map<std::string, std::unordered_map<std::string, int>> omop_unit_count;
    get_omop_unit_counts(omop_unit_count, file_path);
    for(auto omop_id: omop_unit_count) {
        std::string max_unit = "";
        int max_count = 0;
        for(auto unit_count: omop_id.second) {
            if(unit_count.second > max_count) {
                max_count = unit_count.second;
                max_unit = unit_count.first;
            }
        }
        omop_max_units[omop_id.first] = max_unit;
    }
    for(auto omop_id: omop_max_units) {
        std::cout << omop_id.first << " " << omop_id.second << std::endl;
    }
}

void fix_phs(std::string &lab_id,
             std::string &lab_abbrv,
             std::string &lab_unit) {
    std::unordered_set<std::string> phs = {"8522 p-ph", "9010 s-ca-ion", "1530 u-ph", "9225 p-ca-ion", "8869 u-ph-huu", "8423 s-ph", "5098 s-ph(akt)", "586 c++/7.40", "13900 p-ca-ion.", "8523 s-ph"};

    if((phs.find(concat_string({lab_id, lab_abbrv}, " ")) != phs.end()) & (lab_unit == "NA")) lab_unit = "ph";
}

int decide_keep_rows(std::string omop_id,
                     std::string lab_unit,
                     std::unordered_map<std::string, std::string> &omop_max_units) {
    int keep = 0;
    // Special case garvity of urine does not have a unit, howerer 35% of information seems to be in kg/l. Keeping both. 
    if(omop_id == "3019150") {
        if((lab_unit == "kg/l") | (lab_unit == "NA")) {
            keep = 1;
        } else {
            keep = 0;
        }
    // Special case of cases in urine, not relevant 
    } else if(omop_id == "3030467") {
        keep = 0;
    // These are two different ways of counting eosinophiles. Might be a mismatch
    // b-eos and l-eos maps both to this concept
    } else if(omop_id == "3006504") {
        if((lab_unit == "%") | (lab_unit == "e9/l")) {
            keep = 1;
        } else {
            keep = 0;
        }
    // These are two different ways of reporting, it has changes some years ago. 
    // Percentage is b-ghb-a1c and mmol/mol is b-hba1c lab abbreviation
    } else if(omop_id == "3004410") {
        if((lab_unit == "mmol/mol") | (lab_unit == "%")) {
            keep = 1;
        } else {
            keep = 0;
        } 
    }
    // For all others keeping main unit
    if(omop_max_units[omop_id] == lab_unit) {
        keep = 1;
    } else {
        keep = 0;
    }
    return keep;
}
