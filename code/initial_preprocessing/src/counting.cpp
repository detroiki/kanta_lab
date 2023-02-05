#include "header.h"

/*
    Function to analyse missingness in the data file.
    Counts occurance of Puuttuu, -1, "", and NA for each column.
*/
void update_missing_counts(std::vector<std::string> line, 
                           unsigned long long **counts) {
    int idx = 0;
    for(auto col : line) {
        if(col == "Puuttuu") counts[0][idx]++;
        if(col == "-1") counts[1][idx]++;
        if(col == "\"\"") counts[2][idx]++;
        if(col == "NA") counts[3][idx]++;
        idx++;
    }
}

void update_col_tables(std::vector<std::string> line,
                       std::vector<std::string> &col_names,
                       std::unordered_map<std::string, std::unordered_map<std::string, unsigned long long>> &col_tables) {

    for(long unsigned int col_idx=0; col_idx < col_names.size(); col_idx++) {
        std::string col_name = col_names[col_idx];
        std::string col;
        switch(col_idx) {
            case 0: // Individuals and their birth date
                {std::stringstream ss;
                ss << line[col_idx] << "\t" << line[col_idx+1];
                col = ss.str();
                break;}
            case 1:
                break;
            case 3: case 12: // Measurement and Result Completion time
                col = line[col_idx].substr(0, 10);
                break;
            case 7: // Regional ID and their name
                {std::stringstream ss;
                ss << line[col_idx] << "\t" << line[col_idx-1];
                col = ss.str();
                break;}
            case 10: // Regional ID, Lab ID, value
                {std::stringstream ss;
                ss << line[6] << line[7] << "\t" << line[8] << "\t" << line[col_idx] << "\t" << line[col_idx+1];
                col = ss.str();
                break;}       
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