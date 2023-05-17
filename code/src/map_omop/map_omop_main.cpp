#include "../header.h"

int main(int argc, char *argv[]) {
    std::string res_path = argv[1];
    std::string omop_group_id_map_path = argv[2];
    // Out File
    std::vector<std::string> full_res_path_vec = {res_path, "processed/data/all_minimal_omop.csv"};    
    std::string full_res_path = concat_string(full_res_path_vec, std::string(""));


    // OMOP Maps
    std::unordered_map<std::string, std::unordered_map<std::string, std::string>> omop_group_id_map;
    std::unordered_map<std::string, std::string> omop_names;
    read_omop_file(omop_group_id_map_path, omop_group_id_map, omop_names);

    std::ofstream res_file;
    res_file.open(full_res_path); check_out_open(res_file, full_res_path); 
    // Counts
    int first_line = 1;

    // Lines
    std::string line;

    // In
    while(std::getline(std::cin, line)) {
        if(first_line == 1) {
            res_file << "FINREGISTRYID;DATE_TIME;SERVICE_PROVIDER;LAB_ID;LAB_ID_SOURCE;LAB_ABBREVIATION;LAB_VALUE;LAB_UNIT;LAB_ABNORMALITY;OMOP_ID;OMOP_NAME" << "\n";
            first_line = 0;
            continue;
        }
       
        std::vector<std::string> line_vec = split(line, ";");
        std::string finregistryid = line_vec[0];
        std::string date_time = line_vec[1];
        std::string service_provider = line_vec[2];
        std::string lab_id = line_vec[3];
        std::string lab_id_source = line_vec[4];
        std::string lab_abbreviation = line_vec[5];
        std::string lab_value = line_vec[6];
        std::string lab_unit = line_vec[7];
        std::string lab_abnormality = line_vec[8];  

       // Getting current lab source
        std::string omop_lab_source = get_omop_lab_source(lab_id_source, service_provider);
   
        // Finding OMOP mapping

        // Currently identifying the OMOP concept by the lab ID and abbreviation.
        // We can map about 60% of the local lab codes this way
        std::string omop_identifier = concat_string({lab_id, lab_abbreviation}, " ");

        std::string omop_id = get_omop_id(omop_group_id_map, omop_lab_source, omop_identifier);
        std::string omop_name = get_omop_name(omop_id, omop_names);

        // Writing to file
        res_file << finregistryid  << ";" <<  date_time << ";" << service_provider << ";" << lab_id << ";" << lab_id_source << ";" << lab_abbreviation << ";" << lab_value << ";" << lab_unit << ";" <<  lab_abnormality << ";" << omop_id << ";" << omop_name << "\n";
    }

    res_file.close(); 
}

              
