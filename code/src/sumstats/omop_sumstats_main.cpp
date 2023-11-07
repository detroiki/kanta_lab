#include "../header.h"

int main(int argc, char *argv[])
{
    std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();

    std::string res_path = argv[1];

    // Reading in maps for new omop concepts
    std::unordered_map<std::string, std::vector<double>> omops;
    std::unordered_map<std::string, std::unordered_set<std::string>> omop_indvs;

    // Reading
    std::string line;
    int first_line = 1; // Indicates header line
    int n_lines = 0;
    while (std::getline(std::cin, line))
    {
        if (first_line == 1)
        {
            first_line = 0;
            continue;
        }

        std::vector<std::string> line_vec(splitString(line, ','));
        std::string finregid = line_vec[0];
        std::string date_time = line_vec[1];
        std::string service_provider = line_vec[2];
        std::string lab_id = remove_chars(line_vec[3], ' ');
        std::string lab_id_source = line_vec[4];
        std::string lab_abbrv = remove_chars(line_vec[5], ' ');
        std::string lab_value = remove_chars(line_vec[6], ' ');
        std::string lab_unit = remove_chars(line_vec[7], ' ');
        std::string lab_abnorm = remove_chars(line_vec[8], ' ');
        std::string omop_id = line_vec[9];
        std::string omop_name = line_vec[10];

        if (!(omop_id == "NA" || lab_value == "NA"))
        {
            std::string omop_identifier = get_omop_identifier(omop_id, omop_name, lab_unit, std::string("_"));
            omops[omop_identifier].push_back(std::stod(lab_value));
            omop_indvs[omop_identifier].insert(finregid);
        }

        n_lines++;
        if (n_lines % 10000000 == 0)
        {
            std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();

            std::cout << "Lines read = " << n_lines << " Time took = " << std::chrono::duration_cast<std::chrono::minutes>(end - begin).count() << "[min]" << std::endl;
        }
    }

    write_omop_sumstats(omops, omop_indvs, res_path);

    std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();

    std::cout << "Time took = " << std::chrono::duration_cast<std::chrono::minutes>(end - begin).count() << "[min]" << std::endl;
}
