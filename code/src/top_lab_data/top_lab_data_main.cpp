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
