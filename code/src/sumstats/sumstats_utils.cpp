#include "../header.h"

/**
 * @brief writes the omop sumstats to a file
 * 
 * @param omops unordered map of omop identifiers and their values
 * @param res_file file to write to
 * @return void
 * 
 * @details 
 * Writes the omop sumstats to a file. The omop identifiers are split into their components and written to the file. 
 * The omop values are sorted and the mean, median, standard deviation, first and third quantiles, min, max and number
 * of elements are calculated and written to the file.
**/
void write_omop_sumstats(std::unordered_map<std::string, std::vector<double>> omops,
                         std::ofstream &res_file) {
    // write header in all caps
    res_file << "OMOP_ID;OMOP_NAME;LAB_UNIT;MEAN;MEDIAN;SD;FIRST_QUANTILE;THIRD_QUANTILE;MIN;MAX;N_ELEMS\n";

    for(auto omop: omops) {
        std::string omop_identifier = omop.first;
        std::vector<double> values = omop.second;
        std::sort(values.begin(), values.end());

        double mean = get_mean(values);
        double median = get_median(values);
        double sd = get_sd(values, mean);
        double first_quantile = get_quantile(values, double(0.25));
        double third_quantile = get_quantile(values, double(0.75));
        double min = values[0];
        double max = values[values.size()-1];
        int n_elems = values.size();

        std::string omop_id = split(omop_identifier, ";")[0];
        std::string omop_name = split(omop_identifier, ";")[1];
        std::string lab_unit = split(omop_identifier, ";")[2];

        // write to file 
        res_file << omop_id << ";" << omop_name << ";" << lab_unit << ";" << mean << ";" << median << ";" << sd << ";" << first_quantile << ";" << third_quantile << ";" << min << ";" << max << ";" << n_elems << "\n";
    }
}

/**
 * @brief calculates the quantile of a vector of values
 * 
 * @param values vector of values
 * @param quantile quantile to calculate
 * @return double quantile value
 * 
 * @details
 * Calculates the quantile of a vector of values. The vector is sorted and the quantile is calculated using linear interpolation.
*/
double get_quantile(std::vector<double> values, 
                    double quantile) {
    // Step 2: Calculate the position of the quantile
    int n = values.size();
    double position = (n - 1) * quantile; // Using quantile directly

    // Step 3: Find the value at the position with linear interpolation
    int lower_index = static_cast<int>(position);
    int upper_index = lower_index + 1;
    double lower_value = values[lower_index];
    double upper_value = values[upper_index];
    double index_diff = position - lower_index;
    double quantile_value = lower_value + index_diff * (upper_value - lower_value);

    return quantile_value;
}

/**
 * @brief calculates the mean of a vector of values
 * 
 * @param values vector of values
 * @return double mean
 * 
 * @details
 * Calculates the mean of a vector of values.
*/
double get_mean(std::vector<double> values) {
    double sum = std::accumulate(values.begin(), values.end(), 0.0);
    double mean = sum / values.size();
    return(mean);
}

/**
 * @brief calculates the median of a vector of values
 * 
 * @param values vector of values
 * @return double median
 * 
 * @details
 * Calculates the median of a vector of values. If the vector has an even number of elements, the median is calculated
 * as the mean of the two middle elements.
*/
double get_median(std::vector<double> values) {
    double median;
    if (values.size() % 2 == 0) {
        median = (values[values.size()/2 - 1] + values[values.size()/2]) / 2;
    } else {
        median = values[values.size()/2];
    }
    return(median);
}

/**
 * @brief calculates the standard deviation of a vector of values
 * 
 * @param values vector of values
 * @param mean mean of the vector of values
 * 
 * @return double standard deviation
 * 
 * @details
 * Calculates the standard deviation of a vector of values.
*/
double get_sd(std::vector<double> values,
              double mean) {
    double sum_of_squares = 0.0;

    for (const double& value : values) {
        double difference = value - mean;
        sum_of_squares += difference * difference;
    }

    double variance = sum_of_squares / (values.size()-1);
    double sd = std::sqrt(variance);
    return(sd);
}

/**
 * @brief writes the individual omop sumstats to a file
 * 
 * @param indvs_omops_values unordered map of individual omop values
 * @param res_path path to write to
 * 
 * @return void
 * 
 * @details
 * Writes the individual omop sumstats to a file. The omop identifiers are split into their components and written to the file.
 * The omop values are sorted and the mean, median, standard deviation, first and third quantiles, min, max and number
 * of elements are calculated and written to the file.
*/
void write_indvs_omops_sumstats( std::unordered_map<std::string, std::unordered_map<std::string, std::vector<double>>>& indvs_omops_values,
                                std::string res_path
) {    // Opening results file
    std::vector<std::string> full_res_path_vec = {res_path, "indv_omop_lab_avg.csv"};
    std::string full_res_path = concat_string(full_res_path_vec);
    std::ofstream res_file;
    res_file.open(full_res_path);
    check_out_open(res_file, full_res_path);

    // Write header in all caps
    res_file << "FINREGISTRYID;OMOP_ID;LAB_UNIT;MEAN;MEDIAN;SD;FIRST_QUANTILE;THIRD_QUANTILE;MIN;MAX;N_ELEMS" << std::endl;
    for(auto indv: indvs_omops_values) {
        std::string finregid = indv.first;
        std::unordered_map<std::string, std::vector<double>> omop_values = indv.second;
        for(auto omop: omop_values) {
            std::string omop_identifier = omop.first;
            std::vector<double> values = omop.second;
            std::sort(values.begin(), values.end());

            double mean = get_mean(values);
            double median = get_median(values);
            double sd = get_sd(values, mean);
            double first_quantile = get_quantile(values, double(0.25));
            double third_quantile = get_quantile(values, double(0.75));
            double min = values[0];
            double max = values[values.size()-1];
            int n_elems = values.size();

            res_file << finregid << ";" << omop_identifier << ";" << mean << ";" << median << ";" << sd << ";" << first_quantile << ";" << third_quantile << ";" << min << ";" << max << ";" << n_elems << std::endl;
        }
    }
}

void read_indvs_files(std::unordered_map<std::string, std::tuple<date, date>> &relevant_indvs,
                      std::string indvs_path) {
    std::ifstream indvs_file;
    indvs_file.open(indvs_path);
    check_in_open(indvs_file, indvs_path);
    std::string indv_line;

    int first_line = 1;
    while(std::getline(indvs_file, indv_line)) {
        if (first_line == 1) {
            first_line = 0;
            continue;
        }

        std::vector<std::string> line_vec(split(indv_line, ";"));
        std::string finregid = line_vec[0];
        std::string start_date = line_vec[1];
        std::string end_date = line_vec[2];

        date start_date_obj(from_simple_string(start_date));
        date end_date_obj(from_simple_string(end_date));

        std::tuple<date, date> indv_dates(start_date_obj, end_date_obj);

        relevant_indvs[finregid] = indv_dates;
    }
}

void read_omops_file(std::unordered_set<std::string> &relevant_omops,
                     std::string omops_path) {
    std::ifstream omops_file;
    omops_file.open(omops_path);
    check_in_open(omops_file, omops_path);
    std::string omop_line;

    int first_line = 1;
    while(std::getline(omops_file, omop_line)) {
        if (first_line == 1) {
            first_line = 0;
            continue;
        }

        std::vector<std::string> line_vec(split(omop_line, ";"));
        std::string omop_id = line_vec[0];
        std::string lab_unit = line_vec[2];
        std::string omop_identifier = concat_string({omop_id, lab_unit}, std::string(";"));
        relevant_omops.insert(omop_identifier);
    }
}