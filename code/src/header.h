#include <fstream>
#include <sstream>
#include <iostream>
#include <string>
#include <vector>
#include <regex>
#include <unordered_map>
#include <unordered_set>
#include <numeric>
#include <cmath>
#include <algorithm>
#include <chrono>
#include <boost/date_time/gregorian/gregorian.hpp>

using namespace std;
using namespace boost::gregorian;

// Helper functions
char find_delim(std::string file_path);
std::vector<std::string> splitString(const std::string &input, 
                                     char delimiter);
std::vector<std::string> split(const std::string &s, const char *delim);

std::string concat_string(const std::vector<std::string> &elems, std::string delim = std::string(""));
std::string concat_string(const std::unordered_set<std::string> &elems, std::string delim = std::string(""));

void check_out_open(std::ofstream &file_stream, std::string file_path);
int check_in_open(std::ifstream &file_stream, std::string file_path, int stop_if_not_open = 1); 

std::string to_lower(std::string str);
std::string remove_chars(std::string str, char remove_char);
std::string clean_units(std::string lab_unit);

std::string get_omop_info(std::string omop_id, std::string omop_name);
std::string get_lab_info(std::string lab_abbrv, std::string lab_unit);
std::string get_lab_id_omop_info(std::string lab_id, 
                                 std::string lab_info,
                                 std::string omop_info);
std::string get_lab_id_abbrv(std::string lab_id, 
                             std::string lab_abbrv);
std::string get_omop_identifier(std::string lab_id,
                                std::string lab_abbrv,
                                std::string lab_unit,
                                std::string delim = std::string(" "));

// Helper functions for minimal file creation
void fix_nas(std::vector<std::string> &final_line_vec);
std::string get_service_provider_name(std::unordered_map<std::string, std::string> &thl_sote_map,
                               std::string &service_provider_oid);
void get_lab_id_and_source(std::string &local_lab_id,
                           std::string &thl_lab_id,
                           std::string &lab_id,
                           std::string &lab_id_source);
std::string get_lab_abbrv(std::unordered_map<std::string, std::string> &thl_abbrv_map,
                          std::string &lab_id,
                          std::string &lab_id_source,
                          std::string &lab_name);
void write_row_count_report(std::string &report_path,
                            std::string &date,
                            unsigned long long &total_line_count,
                            unsigned long long &valid_line_count,
                            unsigned long long &skip_count,
                            unsigned long long &dup_count,
                            unsigned long long &na_count);
void write_dup_lines_file(std::string &res_path,
                          std::string &file,
                          std::string &date,
                          std::string &report_path,
                          std::unordered_map<std::string, int> &all_dup_lines);

// Writing functions
void write_top_lab_data(std::string file_path,
                        std::string res_path, 
                        std::unordered_set<std::string> keep_omop_ids);
void write_omop_sumstats(std::unordered_map<std::string, std::vector<double>> omops,
                         std::string res_path);
void write_indvs_omops_sumstats( std::unordered_map<std::string, std::unordered_map<std::string, std::vector<double>>>& indvs_omops_values,
                                std::string res_path
                                );

// Reading files function
void get_previous_dup_lines(std::unordered_map<std::string, int> &all_dup_lines, 
                            std::string file,
                            std::string date,
                            std::string res_path);
void read_thl_sote_map(std::unordered_map<std::string, std::string> &thl_sote_map,
                       std::string thl_sote_path);
void read_thl_lab_id_abbrv_map(std::unordered_map<std::string, std::string> &thl_abbrv_map,
                               std::string thl_abbrv_path);
std::vector<std::string> read_correct_lines(std::string &line,
                                            unsigned long long &total_line_count,
                                            unsigned long long &skip_count,
                                            std::ofstream &error_file,
                                            int &lines_valid_status);
void read_omop_file(std::string omop_concept_map_path,
                    std::unordered_map<std::string, std::unordered_map<std::string, std::string>>  &omop_concept_map,
                    std::unordered_map<std::string, std::string> &omop_names);
void get_lab_indv_counts(std::unordered_map<std::string, std::unordered_set<std::string>> &lab_indv_count,
                         std::string file_path);
void read_indvs_files(std::unordered_map<std::string, std::tuple<date, date>> &relevant_indvs,
                      std::string indvs_path);
void get_new_omop_concepts(std::unordered_map<std::string, std::string> &new_omops,
                           std::unordered_map<std::string, std::string> &omop_names,   
                           std::string file_path,
                           int min_count);

// Sumstats
void read_indvs_date_file(std::unordered_map<std::string, std::tuple<date, date>> &relevant_indvs,
                      std::string indvs_path);
void read_indvs_file(std::unordered_set<std::string> &relevant_indvs,
                      std::string indvs_path);
void read_omops_file(std::unordered_set<std::string> &relevant_omops,
                     std::string omops_path);
void read_indvs_omops_sumstats(std::string indvs_omops_sumstats_path,
                               std::unordered_map<std::string, std::unordered_map<std::string, std::unordered_map<std::string, std::string>>> &indvs_omop_sumstats,
                               std::unordered_set<std::string> &relevant_omops,
                               std::unordered_set<std::string> &relevant_indvs);

// Helper functions for OMOP mapping
std::string get_omop_lab_source(std::string lab_id_source,
                                std::string service_provider);
std::string get_omop_id(std::unordered_map<std::string, std::unordered_map<std::string, std::string>> &omop_concept_map,
                        std::string omop_lab_source,
                        std::string omop_identifier);
std::string get_omop_name(std::string omop_id,
                          std::unordered_map<std::string, std::string> &omop_names);

// Helper functions for top OMOP concepts file creation
void get_keep_omop_ids(std::unordered_set<std::string> &keep_omop_ids, 
                       std::unordered_map<std::string, std::unordered_set<std::string>> &lab_indv_count);

// Helpfer functions final fixing
int fix_percentages(std::string &lab_value, 
                    std::string &lab_unit,
                    std::string &lab_abnorm,
                    int keep);
void fix_abnorms(std::string &lab_abnorm);
void shuffle_lab_abnorm_info(std::string &lab_value, 
                             std::string &lab_abnorm, 
                             std::string &lab_unit);
int remove_illegal_values(std::string &lab_value, 
                          std::string &lab_abnorm, 
                          std::string &lab_abbrv,
                          int keep);
void remove_illegal_units(std::string &lab_unit);
void fix_phs(std::string &lab_id,
             std::string &lab_abbrv,
             std::string &lab_unit);
void fix_inrs(std::string &lab_id,
              std::string &lab_abbrv,
              std::string &lab_unit);
void remove_illegal_units(std::string &lab_unit);   
int remove_illegal_measure_year(std::string &date_time,
                                int keep);
void fix_titles(std::string &lab_id,
                  std::string &lab_abbrv,
                  std::string &lab_unit,
                  std::string &lab_value);

// OMOP final fixing
void get_omop_unit_counts(std::unordered_map<std::string, std::unordered_map<std::string, int>> &omop_unit_count,
                          std::string file_path);
void get_omop_max_units(std::unordered_map<std::string, std::string> &omop_max_units,
                        std::string file_path);
int decide_keep_rows(std::string omop_id,
                     std::string lab_unit,
                     std::unordered_map<std::string, std::string> &omop_max_units);

// Math helper function
double get_mean(std::vector<double> values_vec);
double get_median(std::vector<double> values_vec);
double get_sd(std::vector<double> values_vec, double mean);
double get_quantile(std::vector<double> values, double quantile);


// Sumstats config file functions
void read_config_file(std::string config_path,
                      std::unordered_map<std::string, std::string> &configs);
void read_indvs_date_file_from_config(std::unordered_map<std::string, std::string> &configs,
                              std::unordered_map<std::string, std::tuple<date, date>> &relevant_indvs,
                              int stop_if_not_found);
void read_indvs_file_from_config(std::unordered_map<std::string, std::string> &configs,
                                          std::unordered_set<std::string> &relevant_indvs,
                                          int stop_if_not_found);
void read_omops_file_from_config(std::unordered_map<std::string, std::string> &configs,
                              std::unordered_set<std::string> &relevant_omops,
                              int stop_if_not_found);
void read_indvs_omop_sumstats_from_config(std::unordered_map<std::string, std::string> &configs,
                                          std::unordered_map<std::string, std::unordered_map<std::string, std::unordered_map<std::string,  std::string>>> &indvs_omop_sumstats,
                                          std::unordered_set<std::string> &relevant_omops,
                                          std::unordered_set<std::string> &relevant_indvs);
std::vector<std::string> read_relevant_sumstats_from_config(std::unordered_map<std::string, std::string> &configs);



