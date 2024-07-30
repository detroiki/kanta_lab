#include "../header.h"

int main(int argc, char *argv[]) {
    std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();

    std::string res_path = argv[1];
    std::string date = argv[2];
    std::string file_1 = argv[3];

    // merkintaoid, entryoid -> <set of laboratriotutkimusnimikeid, set of paikallinentutkimusnimike, set of tutkimuskoodistonjarjestelmaid, set of tutkimusvastauksentila, set of tutkimustulosarvo, set of tutkimustulosyksikko>
    std::unordered_map<std::string, std::vector<std::unordered_set<std::string>>> all_maps;
    std::unordered_map<std::string, std::vector<int>> all_counts;


    // Flag for first line
    int first_line = 1;
    int n_lines = 0;

    // 0: laboratoriotutkimusnimikeid
    // 32: paikallinentutkimusnimikeid
    // 33: tutkimuskoodistonjarjestelmaid
    // 34: tutkimusvastauksentila
    // 35: tutkimustulosarvo
    // 36: tutkimustulosyksikko
    std::vector<std::size_t> indices = {0, 32, 33, 34, 35, 36};

    // Reading
    std::string line;
    char in_delim;

    while(std::getline(std::cin, line)) {
        if(first_line == 1) {
            first_line = 0;
            continue;
        }
        // Splitting line
        std::vector<std::string> line_vec = split(line, &in_delim);
        //std::string asiakirjaoid = line_vec[1];
        std::string merkintaoid = line_vec[2].substr(9);
        std::string entryoid = line_vec[3].substr(9);

        // concat asiakirjaoid, merkintaoid, entryoid
        std::string oids_concat = concat_string(std::vector<std::string>({merkintaoid, entryoid}), ",");
        // creating empty vector if found for the first time
        if(all_maps.find(oids_concat) == all_maps.end()) {
            std::vector<std::unordered_set<std::string>> empty_vec(6);
            all_maps[oids_concat] = empty_vec;
        }

        // Get a reference to the vector associated with the key oids_concat
        auto& target_vector = all_maps[oids_concat];
        for(auto elem: line_vec) {
            cout << elem << endl;
        }
        // Insert elements into the target_vector using the indices
        for (std::size_t i = 0; i < indices.size(); ++i) {
            cout << i << endl;
            target_vector[i].insert(line_vec[indices[i]]);
        }   

        n_lines++; write_line_update(n_lines, begin, 100000);


    }

    // write all_maps to file
    std::vector<std::string> full_res_path_vec = {res_path, "processed/reports/problem_rows/merkinta_entryoid_data_", date, "_", file_1, ".tsv"};
    std::string full_res_path = concat_string(full_res_path_vec);
    // open file
    std::ofstream res_file;
    if(file_1 == "True") {
        res_file.open(full_res_path); 
    } else {
        res_file.open(full_res_path, std::ios_base::app);
    }
    check_out_open(res_file, full_res_path); 

    // write header
    if(file_1 == "True") {
        res_file << "merkintaoid" << "\t" << "entryoid" << "\t" << "laboratoriotutkimusnimikeid" << "\t" << "paikallinentutkimusnimike" << "\t" << "tutkimuskoodistonjarjestelmaid" << "\t" << "tutkimusvastauksentila" << "\t" << "tutkimustulosarvo" << "\t" << "tutkimustulosyksikko" << "\n";
    }

    // Write data
    for(auto &oids_data: all_maps) {
        res_file << oids_data.first << "\t";
        for(auto &target_data: oids_data.second) {
            for(auto &elem: target_data) {
                res_file << elem << ",";
            }
            res_file << "\t";
        }
        res_file << endl;
    }
}
