#include "../header.h"

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
