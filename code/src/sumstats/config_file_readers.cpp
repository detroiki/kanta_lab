#include "../header.h"

/**
 * @brief Reads config file for the analyses
 * 
 * @param config_path Path to config file
 * @param configs Map for storing config values
 * 
 * @return void
 *
 * 
 * @details Config file can be commented with #, the separator should be either \t, ; or ,
 *          Information in the config file should be:
 *          - KantaLabFile: Path to the KantaLabFile
 *          - SampleFile: Path to the file containing the relevant individuals and the dates
 *          - OmopFile: Path to the file containing the relevant OMOPs
 *          - IndvsOmopSumstatsFile: Path to the file containing the summary statistics for each individual and OMOP
 *          - ResPath: Path to the folder where the results are stored
 *          - RelevantSumstats: Comma separated list of sumstats to be stored in separate files
 *         
*/
void read_config_file(std::string config_path,
                      std::unordered_map<std::string, std::string>& configs) {
    // Open config file
    std::ifstream config_file;
    config_file.open(config_path);
    check_in_open(config_file, config_path);

    // Optional file separators are `\t`, `;` and `,`
    const char* sep = find_separator(config_file);

    std::string config_line;
    while(std::getline(config_file, config_line)) {
        // ignore lines that start with #
        if(config_line[0] != '#') {
            std::vector<std::string> line_vec(split(config_line, sep));
            std::string config_name = line_vec[0];
            std::string config_value = line_vec[1];
            configs[config_name] = config_value;
        }
    }
}

void read_indvs_date_file_from_config(std::unordered_map<std::string, std::string> &configs,
                                       std::unordered_map<std::string, std::tuple<date, date>> &relevant_indvs,
                                       int stop_if_not_found = 0) {
    if(configs.find("SampleFile") != configs.end()) {
        read_indvs_date_file(relevant_indvs, configs["SampleFile"]);
    } else { 
        std::cout << "Could not find SampleFile path from config file " << std::endl; 
        if(stop_if_not_found) exit(EXIT_FAILURE);
        else std::cout << "Using all individuals found in the summary statistics file" << endl;
    }
}


void read_indvs_file_from_config(std::unordered_map<std::string, std::string> &configs,
                                          std::unordered_set<std::string> &relevant_indvs,
                                          int stop_if_not_found = 0) {
  if(configs.find("SampleFile") != configs.end()) {
    read_indvs_file(relevant_indvs, configs["SampleFile"]);
  } else { 
    std::cout << "Could not find SampleFile path from config file " << std::endl; 
    if(stop_if_not_found) exit(EXIT_FAILURE);
    else std::cout << "Using all OMOPs found in the summary statistics file" << endl;
  }
}



void read_omops_file_from_config(std::unordered_map<std::string, std::string> &configs,
                                 std::unordered_set<std::string> &relevant_omops,
                                 int stop_if_not_found = 0) {
    if(configs.find("OmopFile") != configs.end()) {
        read_omops_file(relevant_omops, configs["OmopFile"]);
    } else { 
        std::cout << "Could not find OmopFile path from config file " << std::endl; 
        if(stop_if_not_found) exit(EXIT_FAILURE);
        else std::cout << "Using all OMOPs found in the summary statistics file" << endl;
    }
}

void read_indvs_omop_sumstats_from_config(std::unordered_map<std::string, std::string> &configs,
                                          std::unordered_map<std::string, std::unordered_map<std::string, std::unordered_map<std::string, std::string>>> &indvs_omop_sumstats,
                                          std::unordered_set<std::string> &relevant_omops,
                                          std::unordered_set<std::string> &relevant_indvs) {
    if(configs.find("IndvsOmopSumstatsFile") != configs.end()) {
        read_indvs_omops_sumstats(configs["IndvsOmopSumstatsFile"], indvs_omop_sumstats, relevant_omops, relevant_indvs);
    } else {
        std::cout << "Could not find IndvsOmopSumstatsFile path from config file " << std::endl;
        exit(EXIT_FAILURE);
    }
}

std::vector<std::string> read_relevant_sumstats_from_config(std::unordered_map<std::string, std::string> &configs) {
    std::vector<std::string> relevant_sumstats;
    if(configs.find("OutputEventCount") != configs.end()) relevant_sumstats.push_back("nelems");
    if(configs.find("OutputBinary") != configs.end()) relevant_sumstats.push_back("binary");
    if(configs.find("RelevantSumstats") != configs.end()) {
        std::vector<std::string> relevant_sumstats_vec(split(configs["RelevantSumstats"], ","));
        for(std::string sumstat: relevant_sumstats_vec) {
            relevant_sumstats.push_back(sumstat);
        }
    }
    return(relevant_sumstats);
}