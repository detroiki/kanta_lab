#include "../header.h"

/**
 * */
int main(int argc, char *argv[])
{
    std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();

    std::string entryoid_path = argv[1];
    std::string res_path = argv[2];
    std::string date = argv[3];

    std::unordered_set<std::string> suspect_entryoids;

    // open suspicious entryoid file
    std::ifstream entryoid_file; entryoid_file.open(entryoid_path); check_in_open(entryoid_file, entryoid_path);
    std::string line;
    int first_line = 1; // Indicates header line
    while(std::getline(entryoid_file, line)) {
        if(first_line == 1) { first_line = 0; continue; }
        std::vector<std::string> line_vec = splitString(line, ',');
        suspect_entryoids.insert(line_vec[2]);
    }
    entryoid_file.close();

    // Open results file
    std::vector<std::string> full_res_path_vec = {res_path, "processed/reports/problem_rows/suspect_entryoid_rows_", date, ".csv"};
    std::string full_res_path = concat_string(full_res_path_vec);
    std::ofstream res_file; res_file.open(full_res_path); check_out_open(res_file, full_res_path);
    std::string entryoid;

    // Reading
    first_line=1;
    int n_lines = 0;
    while (std::getline(std::cin, line)) {
        if (first_line == 1) {
            res_file << line << endl; first_line = 0;
        }

        std::vector<std::string> line_vec(splitString(line, ','));
        entryoid = line_vec[3];
        if(suspect_entryoids.find(entryoid) != suspect_entryoids.end()) {
            res_file << line << endl;
        }
        cout << n_lines << ": " << entryoid << endl;
        n_lines++; write_line_update(n_lines, begin, 100);
    }
    res_file.close();
    write_end_run_summary(begin);
}

