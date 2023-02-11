#include "../header.h"

int main(int argc, char *argv[]) {
    // File and delimiter
    const char *delim = ";";
    std::string file_path = argv[1];
    std::string res_path = argv[2];
    std::string sampleid_path = argv[3];
    std::string down_fctr = argv[4];

    // Out File
    std::vector<std::string> full_res_path_vec = {res_path, "processed/data/", "all_minimal_down_", down_fctr, ".csv"};    
    std::string full_res_path = concat_string(full_res_path_vec, std::string(""));


    // Getting ids
    std::ifstream sample_file;
    std::unordered_set<std::string> sample_ids;
    sample_file.open(sampleid_path);
    check_in_open(sample_file, sampleid_path);
    std::string line;
    while(std::getline(sample_file, line)) {
        sample_ids.insert(line);
    }
    sample_file.close();
    
    // Opening
    std::ifstream in_file;
    std::ofstream res_file;
    cout << full_res_path << endl;
    res_file.open(full_res_path); in_file.open(file_path);
    check_out_open(res_file, full_res_path); check_in_open(in_file, file_path); 
    // In
    res_file << "FINNREGISTRYID;DATE;ID;ID_SOURCE;NAME;VALUE;UNIT;ABNORMALITY\n";

    while(std::getline(in_file, line)) {
        std::vector<std::string> line_vec = split(line, delim);
        std::unordered_set<std::string>::const_iterator got = sample_ids.find(line_vec[0]);

        // ID is in sample
        if(got != sample_ids.end()) {
            res_file << line << "\n";
        } 
    }

    // Closing
    res_file.close();
    in_file.close();
}