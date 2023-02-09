#include "../header.h"

int main(int argc, char *argv[]) {
    // File and delimiter
    const char *delim = ";";
    // Defining zero as reading whole file here
    unsigned long long n_rows = 0;
    int n_cols = 25;

    std::string file_path = argv[1];
    std::string file_name = argv[2];
    std::string res_path = argv[3];
    if(argc >= 5) n_cols = atoi(argv[4]);
    if(argc >= 6) n_rows = atoi(argv[5]);

    // Opening
    std::vector<std::string> report_path_vec = {res_path, "processed/reports/problem_rows/", file_name, "_problem_rows_missing.csv"};    
    std::string report_path = concat_string(report_path_vec, std::string(""));
    std::ifstream in_file;
    std::ofstream error_file;
    in_file.open(file_path); error_file.open(report_path);
    check_in_open(in_file, file_path); check_out_open(error_file, report_path);

    // Defining result variables
    std::vector<std::string> col_names;
    // To be able to pass this into functions
    unsigned long long **counts;
    counts = new unsigned long long *[4];
    for(int i = 0; i < 4; i++) counts[i] = new unsigned long long[n_cols] { 0 }; 

    unsigned long long line_count = 0;
    unsigned long long total_line_count = 0;

    int skip_count = 0;
    // Reading file line by line
    std::string line;
    while(std::getline(in_file, line)) {
        if(line.size() > 20) {
            if((line_count > n_rows) & (n_rows != 0)) {
                break;
            }
            // Split values and copy into resulting vector
            std::vector<std::string> line_vec = split(line, delim);
            if(int(line_vec.size()) == n_cols)  {
                // Column names
                if(line_count == 0) {
                    // Copying the first line elements into the column vector
                    std::copy(line_vec.begin(), line_vec.end(), std::back_inserter(col_names));
                } else {
                    // Line analyses
                    update_missing_counts(line_vec, counts);
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
    in_file.close();
    error_file.close();
    // Writing results
    write_missing_res(counts, col_names, n_cols, res_path, file_name);
    write_log(line_count, total_line_count, res_path, file_name);
}