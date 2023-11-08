#include "../header.h"

int main(int argc, char *argv[]) {
    std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();

    std::string in_path = argv[1];
    std::string res_path = argv[2];
    std::string date = argv[3];

    // maps for each column to be counted
    std::unordered_map<std::string, std::unordered_set<std::string>> laboratoriotutkimusnimikeid_map;
    std::unordered_map<std::string, std::unordered_set<std::string>> paikallinentutkimusnimike_map;
    std::unordered_map<std::string, std::unordered_set<std::string>> paikallinentutkimusnimikeid_map;
    std::unordered_map<std::string, std::unordered_set<std::string>> tutkimuskoodistonjarjestelmaid_map;
    std::unordered_map<std::string, std::unordered_set<std::string>> tutkimusvastauksentila_map;
    std::unordered_map<std::string, std::unordered_set<std::string>> tutkimustulosarvo_map;
    std::unordered_map<std::string, std::unordered_set<std::string>> tutkimustulosyksikko_map;

    std::unordered_map<std::string, std::unordered_map<std::string, std::unordered_set<std::string>>> all_maps;
    std::unordered_map<std::string, std::vector<int>> all_counts;

    // Flag for first line
    int first_line = 1;
    int n_lines = 0;

    // do the same but for each column separately
    // open in file
    std::ifstream in_file;
    in_file.open(in_path); check_in_open(in_file, in_path);
    
    // Reading
    std::string line;
    while(std::getline(in_file, line)) {
        if(first_line == 1) {
            first_line = 0;
            continue;
        }

        // Splitting line
        std::vector<std::string> line_vec = splitString(line, ',');
        std::string asiakirjaoid = line_vec[0];
        std::string merkintaoid = line_vec[2];
        std::string entryoid = line_vec[3];


        // concat asiakirjaoid, merkintaoid, entryoid
        std::string oids_concat = concat_string(std::vector<std::string>({asiakirjaoid, merkintaoid, entryoid}), ",");
        // creating distinct sets

        std::string laboratoriotutkimusnimikeid = line_vec[16];
        laboratoriotutkimusnimikeid_map[oids_concat].insert(laboratoriotutkimusnimikeid);
        all_maps[oids_concat]["laboratoriotutkimusnimikeid"].insert(laboratoriotutkimusnimikeid);

        std::string paikallinentutkimusnimike = line_vec[14];
        paikallinentutkimusnimike_map[oids_concat].insert(paikallinentutkimusnimike);
        all_maps[oids_concat]["paikallinentutkimusnimike"].insert(paikallinentutkimusnimike);

        std::string paikallinentutkimusnimikeid = line_vec[15];
        paikallinentutkimusnimikeid_map[oids_concat].insert(paikallinentutkimusnimikeid);
        all_maps[oids_concat]["paikallinentutkimusnimikeid"].insert(paikallinentutkimusnimikeid);

        std::string tutkimuskoodistonjarjestelmaid = line_vec[17];
        tutkimuskoodistonjarjestelmaid_map[oids_concat].insert(tutkimuskoodistonjarjestelmaid);
        all_maps[oids_concat]["tutkimuskoodistonjarjestelmaid"].insert(tutkimuskoodistonjarjestelmaid);

        std::string tutkimusvastauksentila = line_vec[18];
        tutkimusvastauksentila_map[oids_concat].insert(tutkimusvastauksentila);
        all_maps[oids_concat]["tutkimusvastauksentila"].insert(tutkimusvastauksentila);

        std::string tutkimustulosarvo = line_vec[19];
        tutkimustulosarvo_map[oids_concat].insert(tutkimustulosarvo);
        

        n_lines++; write_line_update(n_lines, begin);
    }

    in_file.close();

    // translate the maps to counts
    for(auto &data: laboratoriotutkimusnimikeid_map) {
        std::string oids = data.first;
        all_counts[oids].push_back(laboratoriotutkimusnimikeid_map[oids].size());
    }
    // delete map
    laboratoriotutkimusnimikeid_map.clear();

    // open in file
    in_file.open(in_path); check_in_open(in_file, in_path);

    while(std::getline(in_file, line)) {
        if(first_line == 1) {
            first_line = 0;
            continue;
        }

        // Splitting line
        std::vector<std::string> line_vec = splitString(line, ',');
        std::string asiakirjaoid = line_vec[0];
        std::string merkintaoid = line_vec[2];
        std::string entryoid = line_vec[3];

        std::string paikallinentutkimusnimike = line_vec[14];

        // concat asiakirjaoid, merkintaoid, entryoid
        std::string oids_concat = concat_string(std::vector<std::string>({asiakirjaoid, merkintaoid, entryoid}), ",");
        // creating distinct sets
        paikallinentutkimusnimike_map[oids_concat].insert(paikallinentutkimusnimike);

        n_lines++; write_line_update(n_lines, begin);
    }
    in_file.close();

    // translate the maps to counts
    for(auto &data: paikallinentutkimusnimike_map) {
        std::string oids = data.first;
        all_counts[oids].push_back(paikallinentutkimusnimike_map[oids].size());
    }
    // delete map
    paikallinentutkimusnimike_map.clear();

    // open in file
    in_file.open(in_path); check_in_open(in_file, in_path);

     while(std::getline(in_file, line)) {
        if(first_line == 1) {
            first_line = 0;
            continue;
        }

        // Splitting line
        std::vector<std::string> line_vec = splitString(line, ',');
        std::string asiakirjaoid = line_vec[0];
        std::string merkintaoid = line_vec[2];
        std::string entryoid = line_vec[3];

        std::string paikallinentutkimusnimikeid = line_vec[15];

        // concat asiakirjaoid, merkintaoid, entryoid
        std::string oids_concat = concat_string(std::vector<std::string>({asiakirjaoid, merkintaoid, entryoid}), ",");
        // creating distinct sets
        paikallinentutkimusnimikeid_map[oids_concat].insert(paikallinentutkimusnimikeid);

        n_lines++; write_line_update(n_lines, begin);
    }
    
    in_file.close();

    // translate the maps to counts
    for(auto &data: paikallinentutkimusnimikeid_map) {
        std::string oids = data.first;
        all_counts[oids].push_back(paikallinentutkimusnimikeid_map[oids].size());
    }

    // delete map
    paikallinentutkimusnimikeid_map.clear();

    // open in file
    in_file.open(in_path); check_in_open(in_file, in_path);

    while(std::getline(in_file, line)) {
        if(first_line == 1) {
            first_line = 0;
            continue;
        }

        // Splitting line
        std::vector<std::string> line_vec = splitString(line, ',');
        std::string asiakirjaoid = line_vec[0];
        std::string merkintaoid = line_vec[2];
        std::string entryoid = line_vec[3];

        std::string tutkimuskoodistonjarjestelmaid = line_vec[17];

        // concat asiakirjaoid, merkintaoid, entryoid
        std::string oids_concat = concat_string(std::vector<std::string>({asiakirjaoid, merkintaoid, entryoid}), ",");
        // creating distinct sets
        tutkimuskoodistonjarjestelmaid_map[oids_concat].insert(tutkimuskoodistonjarjestelmaid);

        n_lines++; write_line_update(n_lines, begin);
    }

    in_file.close();

    // translate the maps to counts
    for(auto &data: tutkimuskoodistonjarjestelmaid_map) {
        std::string oids = data.first;
        all_counts[oids].push_back(tutkimuskoodistonjarjestelmaid_map[oids].size());
    }

    // delete map
    tutkimuskoodistonjarjestelmaid_map.clear();

    // open in file
    in_file.open(in_path); check_in_open(in_file, in_path);
    while(std::getline(in_file, line)) {
        if(first_line == 1) {
            first_line = 0;
            continue;
        }

        // Splitting line
        std::vector<std::string> line_vec = splitString(line, ',');
        std::string asiakirjaoid = line_vec[0];
        std::string merkintaoid = line_vec[2];
        std::string entryoid = line_vec[3];

        std::string tutkimusvastauksentila = line_vec[18];

        // concat asiakirjaoid, merkintaoid, entryoid
        std::string oids_concat = concat_string(std::vector<std::string>({asiakirjaoid, merkintaoid, entryoid}), ",");
        // creating distinct sets
        tutkimusvastauksentila_map[oids_concat].insert(tutkimusvastauksentila);
        
        n_lines++; write_line_update(n_lines, begin);
    }

    in_file.close();

    // translate the maps to counts
    for(auto &data: tutkimusvastauksentila_map) {
        std::string oids = data.first;
        all_counts[oids].push_back(tutkimusvastauksentila_map[oids].size());
    }

    // delete map
    tutkimusvastauksentila_map.clear();

    // open in file
    in_file.open(in_path); check_in_open(in_file, in_path);
    
    while(std::getline(in_file, line)) {
        if(first_line == 1) {
            first_line = 0;
            continue;
        }

        // Splitting line
        std::vector<std::string> line_vec = splitString(line, ',');
        std::string asiakirjaoid = line_vec[0];
        std::string merkintaoid = line_vec[2];
        std::string entryoid = line_vec[3];

        std::string tutkimustulosarvo = line_vec[19];

        // concat asiakirjaoid, merkintaoid, entryoid
        std::string oids_concat = concat_string(std::vector<std::string>({asiakirjaoid, merkintaoid, entryoid}), ",");
        // creating distinct sets

        tutkimustulosarvo_map[oids_concat].insert(tutkimustulosarvo);

        n_lines++; write_line_update(n_lines, begin);
    }

    in_file.close();

    // translate the maps to counts
    for(auto &data: tutkimustulosarvo_map) {
        std::string oids = data.first;
        all_counts[oids].push_back(tutkimustulosarvo_map[oids].size());
    }

    // delete map
    tutkimustulosarvo_map.clear();

    // open in file
    in_file.open(in_path); check_in_open(in_file, in_path);
    while(std::getline(in_file, line)) {
        if(first_line == 1) {
            first_line = 0;
            continue;
        }

        // Splitting line
        std::vector<std::string> line_vec = splitString(line, ',');
        std::string asiakirjaoid = line_vec[0];
        std::string merkintaoid = line_vec[2];
        std::string entryoid = line_vec[3];


        std::string tutkimustulosyksikko = line_vec[20];

        // concat asiakirjaoid, merkintaoid, entryoid
        std::string oids_concat = concat_string(std::vector<std::string>({asiakirjaoid, merkintaoid, entryoid}), ",");
        // creating distinct sets
        tutkimustulosyksikko_map[oids_concat].insert(tutkimustulosyksikko);

        n_lines++; write_line_update(n_lines, begin);
    }

    in_file.close();

    // translate the maps to counts
    for(auto &data: tutkimustulosyksikko_map) {
        std::string oids = data.first;
        all_counts[oids].push_back(tutkimustulosyksikko_map[oids].size());
    }

    // delete map
    tutkimustulosyksikko_map.clear();

    // open in file


    cout << "Starting writing results file" << endl;
    // writing te results to the file
    // Results File
    std::vector<std::string> full_res_path_vec = {res_path, "data/suspicious_entryoids_", date, ".csv"};    
    std::string full_res_path = concat_string(full_res_path_vec, std::string(""));
    std::ofstream res_file;
    res_file.open(full_res_path); check_out_open(res_file, full_res_path); 

    // Column headers
    res_file << "ASIAKIRJAOID,MERKINTAOID,ENTRYOID,NR_DISTINCT_LABORATORIOTUTKIMUSNIMIKEID,NR_DISTINCT_PAIKALLINENTUTKIMUSNIMIKE,NR_DISTINCT_PAIKALLINENTUTKIMUSNIMIKEID,NR_DISTINCT_TUTKIMUSKOODISTONJARJESTELMAID,NR_DISTINCT_TUTKIMUSVASTAUKSENTILA,NR_DISTINC_TUTKIMUSTULOSARVO,NR_DISTINCT_TUTKIMUSTULOSYKSIKKO" << "\n";


    // iterating over the set of oids
    for(auto &data: laboratoriotutkimusnimikeid_map) {
        std::string oids = data.first;
        res_file << oids << ",";
        // only print if one of the columns has more than one distinct value
        if((laboratoriotutkimusnimikeid_map[oids].size() > 1) |
            (paikallinentutkimusnimike_map[oids].size() > 1) |
            (paikallinentutkimusnimikeid_map[oids].size() > 1) |
            (tutkimuskoodistonjarjestelmaid_map[oids].size() > 1) |
            (tutkimusvastauksentila_map[oids].size() > 1) |
            (tutkimustulosarvo_map[oids].size() > 1) |
            (tutkimustulosyksikko_map[oids].size() > 1)) {
                res_file << laboratoriotutkimusnimikeid_map[oids].size() << ",";
                res_file << paikallinentutkimusnimike_map[oids].size() << ",";
                res_file << paikallinentutkimusnimikeid_map[oids].size() << ",";
                res_file << tutkimuskoodistonjarjestelmaid_map[oids].size() << ",";
                res_file << tutkimusvastauksentila_map[oids].size() << ",";
                res_file << tutkimustulosarvo_map[oids].size() << ",";
                res_file << tutkimustulosyksikko_map[oids].size() << "\n";
            }
    }

    res_file.close();

    write_end_run_summary(begin);
}
