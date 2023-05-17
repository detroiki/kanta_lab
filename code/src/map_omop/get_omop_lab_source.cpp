#include "../header.h"

std::string get_omop_lab_source(std::string lab_id_source,
                                std::string service_provider) {
    std::string lab_source;
    if(lab_id_source == "0") {
        lab_source = "LABfi";
    } else {
        if(service_provider.find("Helsinki") != std::string::npos) {
            lab_source = "LABfi_HUS";
        } else if(service_provider.find("Tampere") != std::string::npos) {
            lab_source = "LABfi_TMP";
        } else if(service_provider.find("Turku") != std::string::npos) {
            lab_source = "LABfi_TKU";
        } else {
            lab_source = "LABfi_NA";
        }
    }
    return(lab_source);
}