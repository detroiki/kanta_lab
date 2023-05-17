#include "../header.h"

int main(int argc, char *argv[]) {
    std::string file_path = argv[1];
    std::string res_path = argv[2];

    // Counting number of individuals each OMOP concept ID has
    std::unordered_map<std::string, std::unordered_set<std::string>> lab_indv_count;
    get_lab_indv_counts(lab_indv_count, file_path);

    // Getting OMOP IDs to keep with at least 5% of Finregistry individuals
    std::unordered_set<std::string> keep_omop_ids;
    get_keep_omop_ids(keep_omop_ids, lab_indv_count);

    // Writing information from those OMOP Ids to file

}

void get_keep_omop_ids(std::unordered_set<std::string> &keep_omop_ids, 
                       std::unordered_map<std::string, std::unordered_set<std::string>> &lab_indv_count) {
    for(auto omop_id: lab_indv_count) {
        // Min number of individuals (5% of alive in 2021)
        if(omop_id.second.size() > 281008){
            keep_omop_ids.insert(omop_id.first);
        }
    }
}

void write_top_lab_data(std::string file_path,
                        std::string res_path, 
                        std::unordered_set<std::string> keep_omop_ids) {
    /// Opening
    // In file
    std::ifstream in_file;
    in_file.open(file_path); check_in_open(in_file, file_path); 
    // Out file
    std::vector<std::string> full_res_path_vec = {res_path, "processed/data/minimal_omop_top.csv"};    
    std::string full_res_path = concat_string(full_res_path_vec, std::string(""));
    std::ofstream res_file;
    res_file.open(full_res_path); check_out_open(res_file, full_res_path); 

    // Preparing
    std::string line;
    int first_line = 1;

    // Reading
    while(std::getline(in_file, line)) {
        if(first_line == 1) {
            res_file << line << "\n";
            first_line = 0;
            continue;
        }

        std::vector<std::string> line_vec = split(line, ";");
        std::string omop_id = line_vec[9];

        // Writing to file if OMOP ID is in keep set
        if(keep_omop_ids.find(omop_id) != keep_omop_ids.end()) {
            res_file << line << "\n";
        }
    }
}