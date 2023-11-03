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
    std::unordered_set<std::string> phs = {"103268_4 u-tutk-1,u-ph", "0067 u-ph", "11554 s-ph", "50100 s-ph", "50099 fp-ph", "1528 cp-ph", 
        "13039 u-ph-o", "0531 vb-ph","00026 u-ph", "0431 fs-ph", "0685 ab-ph", "0074 ua-ph", "45093 s-ph",
        "627598_5 u-kemseula,u-ph", "247 u-ph", "10193 pf-ph", "45213 uv-ph", "21701 u-ph", "5345 pf-ph", "8591 sp-ph", "1531 vb-ph",
        "21701 u-ph.*", "6001530 u-ph", "50100 fs-ph", "9242 mb-ph", "3667 u-ph", "1527 ab-ph", "3367 u-ph-0", "10099 u-ph",
         "50099 p-ph", "103 b-ph", "0526 fs-ph", "600334 u-ph", "1600334 u-ph", "8522 p-ph", "9010 s-ca-ion", "1530 u-ph",
          "9225 p-ca-ion", "8869 u-ph-huu", "8423 s-ph", "5098 s-ph(akt)", "586 c++/7.40", "13900 p-ca-ion.", "8523 s-ph",
          "0320 ab-ph", "0431 uv-ph", "1528 cb-ph", "602090 u-ph", "2600334 u-ph", "14234 cb-phpika", "0410 ab-ph", "0341 vb-ph",
          "50292 di-ph", "8910 uv-ph", "3667 u-ph-o", "9035 pf-ph", "627598_5 u-kemseul,u-ph", "50099 fs-ph", 
           "0007 ab-ph(t)", "1364 u-phv", "1531 vp-ph", "8025 pf-ph", "0056 u-ph-huu", "14230 vb-phpika", "0303 u-ph-o*", "0103 b-ph",
           "11570 vb-ph-vt", "8270 vb-ph", "9452 ab-ph", "10730 ab-ph", "0415 cb-ph", "1531 b-ph", "0695 cb-ph",  "0225 ub-ph",
            "45094 p-ph", "0918 cb-ph", "103667 u-ph-o", "0528 cb-ph", "602556_4 pikau-sti,u-ph", "8591 -ph", "602556_4 pikau-stix,u-ph",
             "0614 u-ph", "0375 u-ph*", "606025 vb-ph", "8915 ua-ph", "50056 pf-ph", "17261 ub-ph", "11128 ab-ph(t)", "0683 fs-ph",
             "0325 u-ph", "45219 ua-ph", "595 ph", "00025 u-ph", "12130 fl-ph", "50161 uv-ph", "610 pf-ph", "20032 pf-ph", "1532 zb-ph",
           "5098 s-ph", "0350 u-ph", "21701 u-ph.", "0375 u-ph", "2103667 u-ph-o", "11554 s-ph*", "9018 -ph", "0061 ab-ph(t)",
           "2431 pf-ph", "50160 ua-ph", "101543_1 vb-he-tase,vb-ph", "5402 mb-ph", "0368 u-ph-hy", "0105 u-ph", "1527 ap-ph"};

    if((phs.find(concat_string(std::vector<std::string>({lab_id, lab_abbrv}), " ")) != phs.end()) & (lab_unit == "NA")) lab_unit = "ph";
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
