#include "../header.h"

std::string get_omop_id(std::unordered_map<std::string, std::unordered_map<std::string, std::string>> &omop_group_id_map,
                        std::string omop_lab_source,
                        std::string omop_identifier){
    std::string omop_id;
    // Code source is known
    if(omop_lab_source != "LABfi_NA") {
        // Checking if we also know the actual Lab ID abbreviation pair
        if(omop_group_id_map[omop_lab_source].find(omop_identifier) != omop_group_id_map[omop_lab_source].end()) {
            omop_id = omop_group_id_map[omop_lab_source][omop_identifier];
        // Cannot map
        } else {
            omop_id = "NA";
        }
    } else {
        // Currently using a hierarchical approach to mapping for the local codes outside of the
        // major hospitals  HUS (Helsinki), TMP (Tampere), TKU (Turku) - in that order
        if(omop_group_id_map["LABfi_HUS"].find(omop_identifier) != omop_group_id_map["LABfi_HUS"].end()) {
            omop_id = omop_group_id_map["LABfi_HUS"][omop_identifier];
        } else if(omop_group_id_map["LABfi_TMP"].find(omop_identifier) != omop_group_id_map["LABfi_TMP"].end()) {
            omop_id = omop_group_id_map["LABfi_HUS"][omop_identifier];
        } else if(omop_group_id_map["LABfi_TKU"].find(omop_identifier) != omop_group_id_map["LABfi_TKU"].end()) {
            omop_id = omop_group_id_map["LABfi_HUS"][omop_identifier];
        // Cannot map
        } else {
            omop_id = "NA"; 
        }  
    }
    return(omop_id);
}

std::string get_omop_name(std::string omop_id,
                          std::unordered_map<std::string, std::string> &omop_names){
    std::string omop_name;
    if(omop_id == "NA") {
        omop_name = "NA";
    } else {
        if(omop_names.find(omop_id) != omop_names.end()) {
            omop_name = omop_names[omop_id];
        } else {
            omop_name = "NA";
        }
    }
    return(omop_name);
}
            