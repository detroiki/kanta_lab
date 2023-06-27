#include "../header.h"

void fix_phs(std::string &lab_id,
             std::string &lab_abbrv,
             std::string &lab_unit) {
    std::unordered_set<std::string> phs = {"8522 p-ph", "9010 s-ca-ion", "1530 u-ph", "9225 p-ca-ion", "8869 u-ph-huu", "8423 s-ph", "5098 s-ph(akt)", "586 c++/7.40", "13900 p-ca-ion.", "8523 s-ph"};

    if((phs.find(concat_string({lab_id, lab_abbrv}, " ")) != phs.end()) & (lab_unit == "NA")) lab_unit = "ph";
}

int remove_titles(std::string &lab_id,
                   std::string &lab_abbrv,
                   int keep) {
    std::unordered_set<std::string> titles = {"2474 b-pvk-t", "2473 b-pvk", "2474 b-pvkt"}; 
    //"90 b-pvk+t", "139 u-bakt", "147 vekapak1", "158 b-diffi", "185 u-solut", "248 pt-gfre-md", "273 b-pvk+tkd,ig", "198 fp-lipidit", "316 pu-baktvi1", "412 fs-lipidit", "437 u-tutk-1,ph", "452 b-tvk", "462 fp-kol-ind", "471 b-pvk", "516 b-pvk+tmd", "559 2hgluk", "568 vb-he-tase", "618 s-hbvpak", "672 s-keliseu", "695 pt-gluk-r1", "723 cb-het-ion", "760 b-baktivi", "761 s-urokaer", "777 pt-gluk-2h", "798 vb-vkperus", "803 u-kemseul,ph", "852 pt-kt/v1", "958 cb-bepika", "983 pocabrc", "993 vb-vklaaja"};

    if((titles.find(concat_string({lab_id, lab_abbrv}, " ")) != titles.end())) keep = 0;
    return(keep);
}

void fix_inrs(std::string &lab_id,
              std::string &lab_abbrv,
              std::string &lab_unit) {
    std::unordered_set<std::string> inrs = {"4520 p-tt-inr", "4520 p-inr", "955 p-inr."};

    if((inrs.find(concat_string({lab_id, lab_abbrv}, " ")) != inrs.end()) & (lab_unit == "NA")) lab_unit = "inr";
}

void remove_illegal_units(std::string &lab_unit) {
    try {
        double lab_unit_double = std::stod(lab_unit);
        lab_unit = "NA";
    } catch(...) {
        return;
    }
}

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