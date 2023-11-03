#include "../header.h"

/**
 * @brief Fixes phs that often have no units
 * 
 * @param lab_id The lab id of the lab test
 * @param lab_abbrv The lab abbreviation of the lab test
 * @param lab_unit The lab unit of the lab test
 * 
 * @return void
 * 
 * Makes the units "ph" to preserve the information that this is a ph measurement.
*/
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


/**
 * @brief Fixes titles that have random units and values even though they are not measurements
 * 
 * @param lab_id The lab id of the lab test
 * @param lab_abbrv The lab abbreviation of the lab test
 * @param lab_unit The lab unit of the lab test
 * @param lab_value The lab value of the lab test
 * 
 * @return void
 * 
 * Makes the units "ordered" to preserve the information that this title has been ordered for the patient.
*/
void fix_titles(std::string &lab_id,
                  std::string &lab_abbrv,
                  std::string &lab_unit,
                  std::string &lab_value) {
    std::unordered_set<std::string> titles = {"2474 b-pvk-t", "2473 b-pvk", "2474 b-pvkt", "90 b-pvk+t", "139 u-bakt", "147 vekapak1", 
                                              "158 b-diffi", "185 u-solut", "248 pt-gfre-md", "273 b-pvk+tkd,ig",
                                              "298 fp-lipidit", "316 pu-baktvi1", "412 fs-lipidit", "437 u-tutk-1,ph", "452 b-tvk", 
                                              "462 fp-kol-ind", "471 b-pvk", "516 b-pvk+tmd", "559 2hgluk", "568 vb-he-tase"
                                              "618 s-hbvpak", "672 s-keliseu", "695 pt-gluk-r1", "723 cb-het-ion", "760 b-baktjvi",
                                              "761 s-ruokaer", "774 b-pvk+tkd", "777 pt-gluk-2h", "798 vb-vkperus", "803 u-kemseul,ph", 
                                              "852 pt-kt/v1", "958 cb-bepika", "983 pocabrc", "993 vb-vklaaja", "994 pes.j‰l",
                                              "2475 b-pvk+tkd"};
    if((titles.find(concat_string(std::vector<std::string>({lab_id, lab_abbrv}), " ")) != titles.end())) {
        lab_unit = "ordered";
        lab_value = "NA";
    }
}

/**
 * @brief Converts units from e6/l to e9/l
 * 
 * @param lab_value The lab value of the lab test
 * @param lab_unit The lab unit of the lab test
 * 
 * @return void
 * 
 * Converts units from e6/l to e9/l.
*/
void unit_conversion(std::string lab_value,
                     std::string lab_unit) {
    // Convert e6/l to e9/l
    if(lab_unit == "e6/l") {
        double lab_value_double = std::stod(lab_value);
        lab_value_double = lab_value_double / 1000;
        lab_value = std::to_string(lab_value_double);
        lab_unit = "e9/l";
    }
}

/**
 * @brief Fixes inrs that often have no units
 * 
 * @param lab_id The lab id of the lab test
 * @param lab_abbrv The lab abbreviation of the lab test
 * @param lab_unit The lab unit of the lab test
 * 
 * @return void
 * 
 * Makes the units "inr" to preserve the information that this is an inr measurement.
*/
void fix_inrs(std::string &lab_id,
              std::string &lab_abbrv,
              std::string &lab_unit) {
    std::unordered_set<std::string> inrs = {"4520 p-tt-inr", "4520 p-inr", "955 p-inr."};

    if((inrs.find(concat_string(std::vector<std::string>({lab_id, lab_abbrv}), " ")) != inrs.end()) & (lab_unit == "NA")) lab_unit = "inr";
}

/**
 * @brief Removes illegal units that are actually numbers
 * 
 * @param lab_unit The lab unit of the lab test
 * 
 * @return void
*/
void remove_illegal_units(std::string &lab_unit) {
    try {
        double lab_unit_double = std::stod(lab_unit);
        lab_unit = "NA";
    } catch(...) {
        return;
    }
}

/**
 * @brief Removes illegal measure years that are before 2014
 * 
 * @param date_time The date and time of the lab test
 * @param keep Whether the line should be kept or not
 * 
 * @return void
 * 
 * If the year of the lab test is before 2014, the line is removed.
*/
int remove_illegal_measure_year(std::string &date_time,
                                int keep) {
    std::string year = date_time.substr(0, 4);
    try {
        int year_int = std::stoi(year);
        if(year_int < 2014) {
            keep = 0;
        } 
    } catch(...) {
        keep = 0;
    }
    return(keep);
}

/**
 * @brief Removes illegal values that are not numbers
 * 
 * @param lab_value The lab value of the lab test
 * @param lab_abnorm The lab abnormality of the lab test
 * @param lab_abbrv The lab abbreviation of the lab test
 * @param keep Whether the line should be kept or not
 * 
 * @return void
 * 
 * If the lab value is not a number, the line is removed. Additionally, negative values
 * are removed except for -h-ind, ab-hb-met, be and vekaas.
*/
int remove_illegal_values(std::string &lab_value, 
                          std::string &lab_abnorm, 
                          std::string &lab_abbrv,
                          int keep) {
    try {
        double lab_value_double = std::stod(lab_value);
        // Special cases of lab values that can be negative
        if((lab_abbrv == "-h-ind")) return(keep);
        if((lab_abbrv == "ab-hb-met")) return(keep);
        if(lab_abbrv.length() > 3) {
            if((lab_abbrv.substr(3, 2) == "be")) return(keep);
        } 
        if(lab_abbrv.length() >= 9) {
            if((lab_abbrv.substr(3, 6) == "vekaas")) return(keep);
        }
        // All other values should be positive
        if(lab_value_double < 0) {
            lab_value = "NA";
            // This means we end up having neither lab value nor abnormality
            if(lab_abnorm == "NA") keep = 0;  
        }
    // This means the lab value is not a number
    } catch(const std::invalid_argument& e) {
        lab_value = "NA";
        // This means we end up having neither lab value nor abnormality
        if(lab_abnorm == "NA") keep = 0;
    }
    return(keep);
}

/**
 * @brief Fixes percentages that are in osuus (fraction) format
 * 
 * @param lab_value The lab value of the lab test
 * @param lab_unit The lab unit of the lab test
 * @param lab_abnorm The lab abnormality of the lab test
 * @param keep Whether the line should be kept or not
 * 
 * @return int
 * 
 * If the lab unit is osuus (fraction), the lab value is multiplied by 100 and the lab unit
 * turned to %. If the lab unit is already %, the lab value is kept as is.
*/
int fix_percentages(std::string &lab_value, 
                    std::string &lab_unit,
                    std::string &lab_abnorm,
                    int keep) {
    // Make osuus (fraction) and % the same
    if(lab_unit == "osuus") {
        lab_unit = "%";
        try {
            lab_value = std::to_string(std::stod(lab_value) * 100);
        } catch(const std::invalid_argument& e) {
            lab_value = "NA";
            // This means we end up having neither lab value nor abnormality
            if(lab_abnorm == "NA") keep = 0;
        }
    }

    return(keep);
}

/**
 * @brief Fixes abnormality abbreviations to be consistent
 * 
 * @param lab_abnorm The lab abnormality of the lab test
 * 
 * @return void
 * 
 * Fixes abnormality abbreviations to be consistent with the OMOP standard. This means replacing
 * < with L, > with H, POS with A and NEG with N. If the abbreviation is not one of these, it is
 * replaced with NA.
*/
void fix_abnorms(std::string &lab_abnorm) {
    if(lab_abnorm == "<") {
        lab_abnorm = "L";
    } else if(lab_abnorm == ">") {
        lab_abnorm = "H";
    } else if(lab_abnorm == "POS") {
        lab_abnorm = "A";
    } else if(lab_abnorm == "NEG") {
        lab_abnorm = "N";
    }
    
    if((lab_abnorm != "A") & (lab_abnorm != "AA") & (lab_abnorm != "H") & (lab_abnorm != "HH") & (lab_abnorm != "L") & (lab_abnorm != "LL") & (lab_abnorm != "N")) {
        lab_abnorm = "NA";
    }
}

/**
 * @brief Shuffles lab abnormality information to the correct columns
 * 
 * @param lab_value The value of the lab test
 * @param lab_abnorm The abnormality of the lab test
 * @param lab_unit The unit of the lab test
 * 
 * @return void
 * 
 * Moves lab unit information on abnormality to lab abnormality column and 
 * lab abnormality information to lab value column for binary tests where
 * abnormality is the only information.
*/
void shuffle_lab_abnorm_info(std::string &lab_value, 
                             std::string &lab_abnorm, 
                             std::string &lab_unit) {
    // Moving lab unit information on abnormality to lab abnormality column
    if((lab_unit == "A")) {
        lab_abnorm = "A";
        lab_unit = "NA";
    } else if(lab_unit == "N") {
        lab_abnorm = "N";
        lab_unit = "NA";
    } 

    // Moving lab abnormality information to lab value column for binary tests
    // where abnormality is the only information
    if(lab_value == "NA") {
        if((lab_abnorm == "A") | (lab_abnorm == "AA") | (lab_abnorm == "L") | (lab_abnorm == "LL") | (lab_abnorm == "H") | (lab_abnorm == "HH")) {
            lab_value = "1";
            lab_unit = "binary";
        } else if(lab_abnorm == "N") {
            lab_value = "0";
            lab_unit = "binary";
        } 
    }
}