#include "../header.h"

int main(int argc, char *argv[]) {
    std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();

    std::string res_path = argv[1];
    std::string date = argv[2];
    std::string file_1 = argv[3];

    std::unordered_map<std::string, std::vector<std::unordered_set<std::string>>> all_maps;
    std::unordered_map<std::string, std::vector<int>> all_counts;


    // Flag for first line
    int first_line = 1;
    int n_lines = 0;

    // 0: laboratoriotutkimusnimikeid
    // 31: paikallinentutkimusnimike
    // 33: tutkimuskoodistonjarjestelmaid
    // 34: tutkimusvastauksentila
    // 35: tutkimustulosarvo
    // 36: tutkimustulosyksikko
    std::vector<std::size_t> indices = {0, 31, 33, 34, 35, 36};

    // Reading
    std::string line;
    char in_delim = '\t';

    while(std::getline(std::cin, line)) {
        if(first_line == 1) {
            first_line = 0;
            continue;
        }
        // Splitting line
        std::vector<std::string> line_vec = split(line, &in_delim);
        //std::string asiakirjaoid = line_vec[1];
        //std::string merkintaoid = line_vec[2];
        std::string entryoid = line_vec[3];

        // concat asiakirjaoid, merkintaoid, entryoid
        //std::string oids_concat = concat_string(std::vector<std::string>({asiakirjaoid, merkintaoid, entryoid}), ",");
        // creating empty vector if found for the first time
        if(all_maps.find(entryoid) == all_maps.end()) {
            std::vector<std::unordered_set<std::string>> empty_vec(7);
            all_maps[entryoid] = empty_vec;
        }

        // Get a reference to the vector associated with the key oids_concat
        auto& target_vector = all_maps[entryoid];
        // Insert elements into the target_vector using the indices
        for (std::size_t i = 0; i < indices.size(); ++i) {
            target_vector[i].insert(line_vec[indices[i]]);
        }   

        n_lines++; write_line_update(n_lines, begin, 100000);
    }

    // translate the maps to counts
    for(auto &oids_data: all_maps) {
        std::string oids = oids_data.first;
        for(long unsigned int elem_idx = 0; elem_idx < oids_data.second.size(); elem_idx++) {
            all_counts[oids].push_back(oids_data.second[elem_idx].size());
        }
    }
    // delete map
    all_maps.clear();
        
    cout << "Starting writing results file" << endl;
    std::vector<std::string> full_res_path_vec = {res_path, "data/suspicious_entryoids_", date, ".csv"};    
    std::string full_res_path = concat_string(full_res_path_vec, std::string(""));
    std::ofstream res_file;
    if(file_1 == "True") {
        res_file.open(full_res_path); 
    } else {
        res_file.open(full_res_path, std::ios_base::app);
    }
    check_out_open(res_file, full_res_path); 

    if(file_1 == "True") {
        // Column headers
        res_file << "ASIAKIRJAOID,MERKINTAOID,ENTRYOID,NR_DISTINCT_LABORATORIOTUTKIMUSNIMIKEID,NR_DISTINCT_PAIKALLINENTUTKIMUSNIMIKE,NR_DISTINCT_TUTKIMUSKOODISTONJARJESTELMAID,NR_DISTINCT_TUTKIMUSVASTAUKSENTILA,NR_DISTINC_TUTKIMUSTULOSARVO,NR_DISTINCT_TUTKIMUSTULOSYKSIKKO" << "\n";
    }
    // iterating over the set of oids
    for(auto &data: all_counts) {
        std::string oids = data.first;
        // only print if one of the columns has more than one distinct value
        if((all_counts[oids][0] > 1) |
            (all_counts[oids][1] > 1) |
            (all_counts[oids][2] > 1) |
            (all_counts[oids][3] > 1) |
            (all_counts[oids][4] > 1) |
            (all_counts[oids][5] > 1)) {
                res_file << oids << ",";
                res_file << all_counts[oids][0] << ",";
                res_file << all_counts[oids][1] << ",";
                res_file << all_counts[oids][2] << ",";
                res_file << all_counts[oids][3] << ",";
                res_file << all_counts[oids][4] << ",";
                res_file << all_counts[oids][5] << ",";
                res_file << all_counts[oids][6] << "\n";
            }
    }
    all_counts.clear();
    write_end_run_summary(begin);
    cout << endl;
    res_file.close();
}
