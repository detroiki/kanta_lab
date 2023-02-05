#include "header.h"

int main(int argc, char *argv[]) {
    // File and delimiter
    
    std::string file_name;
    const char *delim = ";";
    // Defining zero as reading whole file here
    unsigned long long n_rows = 0;
    int n_cols = 15;

    if(argc >= 2) file_name = argv[1];
    if(argc >= 3) n_rows = atoi(argv[2]);
    if(argc >= 4) n_cols = atoi(argv[3]);

    std::ifstream my_file;
    std::ofstream error_file;

    // Opening
    my_file.open(file_name); error_file.open("data/problem_rows.csv");
    if(!my_file.is_open())  {
        perror("Error when opening file.");
        exit(EXIT_FAILURE);
    }
    if(!error_file.is_open())  {
        perror("Error when opening error file.");
        exit(EXIT_FAILURE);
    }

    // Defining result variables
    std::vector<std::string> col_names;
    // To be able to pass this into functions
    unsigned long long **counts;
    counts = new unsigned long long *[4];
    for(int i = 0; i < 4; i++)
        counts[i] = new unsigned long long[n_cols] { 0 }; 
    // Table of each value for the different columns
    std::unordered_map<std::string, std::unordered_map<std::string, unsigned long long>> col_tables; 
    unsigned long long line_count = 0;
    unsigned long long total_line_count = 0;

    int skip_count = 0;
    // Reading file line by line
    std::string line;

    while(std::getline(my_file, line)) {
        if(line.size() > 20) {
            if((line_count > n_rows) & (n_rows != 0)) {
                break;
            }
            // Split values and copy into resulting vector
            std::vector<std::string> line_vec = split(line, delim);
            if(line_vec.size() == n_cols)  {
                // Column names
                if(line_count == 0) {
                    // Copying the first line elements into the column vector
                    std::copy(line_vec.begin(), line_vec.end(), std::back_inserter(col_names));
                } else {
                    // Line analyses
                    update_missing_counts(line_vec, counts);
                    //
                    //update_col_tables(line_vec, col_names, col_tables);
                }
                line_count++;
                total_line_count++;
            } else {
                cout << "Skipping line: " << total_line_count << " size: " << line.size() << " no of columns: " << line_vec.size() << " " << line << endl;
                skip_count++;
                total_line_count++;
                error_file << line << "\n";
            }
        } else {
            cout << "Skipping line: " << total_line_count << " size: " << line.size() << " " << line << endl;
            error_file << line << "\n";
            skip_count++;
            total_line_count++;
        }
    }
    cout << "line number: " << line_count << " closing" << endl;
    cout << "skipped: " << skip_count << endl;
    // Closing
    my_file.close();
    error_file.close();
    // Writing results
    if(line_count > 10) {
        write_missing_res(counts, col_names, n_cols);
        write_log(line_count, total_line_count);
        //write_col_counts(col_tables, col_names, n_cols);
    }
}

