#include "header.h"

void write_missing_res(unsigned long long **counts,
                       std::vector<std::string> col_names,
                       int n_cols) {
    // Opening results file
    ofstream res_file;
    res_file.open("results/missing_report.tsv");
    if(!res_file.is_open())  {
        perror("Error when opening missing_report.tsv file.");
        exit(EXIT_FAILURE);
    }


    // Column and Row informations
    std::vector<std::string> missing_types = { "Puuttuu", "-1", "Empty", "NA"};
    res_file << "Type\t";
    for(int col_idx=0; col_idx < n_cols; col_idx++) {
        res_file << col_names[col_idx] << "\t";
    }
    res_file << "\n";

    // Writing counts
    for(int row_idx=0; row_idx < 4; row_idx++) {
        res_file << missing_types[row_idx] << "\t";
        for(int col_idx=0; col_idx < n_cols; col_idx++) {
            res_file << counts[row_idx][col_idx] << "\t";
        }
        res_file << "\n";
    }

    // Closing
    res_file.close();
}

void write_log(unsigned long long line_count, 
               unsigned long long total_line_count) {
    // Opening
    ofstream res_file;
    res_file.open("results/missing.txt");
    if(!res_file.is_open())  {
        perror("Error when opening missing.txt file.");
        exit(EXIT_FAILURE);
    }
    res_file << "Total number of rows: " << total_line_count-1 << "\n" << "Number of used rows: " << line_count-1 << endl;

    // Closing
    res_file.close();
}

void write_col_counts(std::unordered_map<std::string, std::unordered_map<std::string, unsigned long long>> col_tables,
                       std::vector<std::string> col_names,
                       int n_cols) {
    // Opening results file

    //cout << "Puuttuu " << &(col_tables[0])["Puuttuu"] << endl;
    for(auto col_name: col_names) {
        std::stringstream ss;
        ss << "results/" << col_name << "_table.tsv";
        std::string file_name = ss.str();
        ofstream res_file;
        res_file.open(file_name);
        if(!res_file.is_open())  {
            cout << col_name << endl;
            perror("Error when opening table file.");
            exit(EXIT_FAILURE);
        }
        for(const std::pair<const std::string, unsigned long long>& elem: col_tables[col_name]) {
               res_file << elem.first << "\t" << elem.second << "\n";
         }
        res_file.close();
    }

}
