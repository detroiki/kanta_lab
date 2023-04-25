#include "../header.h"

void update_col_tabs(std::vector<std::string> line,
                       std::vector<std::string> &col_names,
                       std::unordered_map<std::string, std::unordered_map<std::string, unsigned long long>> &col_tables) {

    for(long unsigned int col_idx=0; col_idx < col_names.size(); col_idx++) {
        std::string col_name = col_names[col_idx];
        std::string col;

        switch(col_idx) {
            case 1: case 5: case 9:
                break;
            default:
                col = line[col_idx];
        }
        if(col_tables[col_name].count(col) != 1){
            col_tables[col_name][col] = 1;
        } else {
            col_tables[col_name][col]++;
        }
    }
}


void update_col_tabs(std::vector<std::string> line,
                       std::vector<std::string> &col_names,
                       std::unordered_map<std::string, std::unordered_map<std::string, unsigned long long>> &col_tables) {

    for(long unsigned int col_idx=0; col_idx < col_names.size(); col_idx++) {
        std::string col_name = col_names[col_idx];
        std::string col;
        std::string finregistryid = line[0];
        std::string date_time = line[1];
        std::string service_provider = line[2];
        std::string lab_id = line[3];
        std::string lab_abbreviation = line[4];
        std::string lab_value = line[5];
        std::string lab_unit = line[6];
        std::string lab_abnormality = line[7];
        std::string omop_id = line[8];
        std::string omop_name = line[9];
        std::string omop_abbreviation = line[10];
        std::string omop_unit = line[11];

        switch(col_idx) {
            // Individual, time, lab id, omop id
            case 0: // Individuals and their lab date
                {std::stringstream ss;
                ss << finregistryid << "\t" << date_time << "\t" << lab_id << "\t" << omop_id;
                col = ss.str();
                break;}
            // Lab id, omop ID, lab abbreviation, lab value, lab unit
            case 5: 
                {std::stringstream ss;
                ss << lab_id << "\t" << omop_id << "\t" << lab_abbreviation << "\t" << lab_value << "\t" << lab_unit;
                col = ss.str();
                break;}
            // Cross tabulation lab ID and OMOP ID
            case 8:
                {std::stringstream ss;
                ss << lab_id << "\t" << omop_id;
                col = ss.str();
                break;}
            // Cross tabulation lab abbreviation and OMOP abbreviation
            case 10:
                {std::stringstream ss;
                ss << lab_abbreviation << "\t" << omop_abbreviation;
                col = ss.str();
                break;}
            // Cross tabulation lab unit and OMOP unit
            case 11:
                {std::stringstream ss;
                ss << lab_unit << "\t" << omop_unit;
                col = ss.str();
                break;}
            // omop ID, OMOP abbreviation, lab value, OMOP unit, service provider
            case 9: 
                {std::stringstream ss;
                ss << omop_id << "\t" << omop_abbreviation << "\t" << lab_value << "\t" << omop_unit << "\t" << service_provider;
                col = ss.str();
                break;}
            case 1: case 2: case 3: case 7: case 9: 
                break;
            default:
                col = line[col_idx];
        }
        if(col_tables[col_name].count(col) != 1){
            col_tables[col_name][col] = 1;
        } else {
            col_tables[col_name][col]++;
        }
    }
}