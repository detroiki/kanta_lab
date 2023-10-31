#include "header.h"

/**
 * @brief Checks whether the std::ofstream is open
 * 
 * @param file_stream std::ofstream to be checked
 * @param file_path Path to the file
 * 
 * @return void
 * 
 * @note Exits the program if the file is not open and prints the error message.
*/
void check_out_open(std::ofstream& file_stream, 
                    std::string file_path) {
    if(!file_stream.is_open())  {
        std::vector<std::string> error_msg_vec = {"Error when opening ", file_path};
        std::string error_msg = concat_string(error_msg_vec);
        const char* error_msg_char = error_msg.c_str();
        perror(error_msg_char);
        exit(EXIT_FAILURE);
    }
}

/**
 * @brief Checks whether the std::ifstream is open
 * 
 * @param file_stream std::ifstream to be checked
 * @param file_path Path to the file
 * 
 * @return void
 * 
 * @note Exits the program if the file is not open and prints the error message.
*/
void check_in_open(std::ifstream &file_stream, 
                   std::string file_path) {
    if(!file_stream.is_open())  {
        std::vector<std::string> error_msg_vec = {"Error when opening ", file_path};
        std::string error_msg = concat_string(error_msg_vec);
        const char* error_msg_char = error_msg.c_str();
        perror(error_msg_char);
        exit(EXIT_FAILURE);
    }
}

/**
 * @brief Learns the separator of a file
 * 
 * @param config_file File to be read
 * 
 * @return const char* Separator
*/
const char* find_separator(std::ifstream &config_file) {
    const char* sep;

    std::string config_line;

    int line = 0;
    while(std::getline(config_file, config_line)) {
        // Ignore lines that start with #
        if(config_line[0] != '#') {
            if(line == 0) {
                line++;
                if(config_line.find("\t") != std::string::npos) {
                    sep = "\t";
                    break;
                } else if(config_line.find(";") != std::string::npos) {
                    sep = ";";
                    break;
                } else if(config_line.find(",") != std::string::npos) {
                    sep = ";";
                    break;
                } else {
                    std::cout << "Could not find separator from config file. Tried `\t`, `,` and `;`" << std::endl;
                    std::cout << "If you are using \t as a separator make sure you are not accidentally using multiple spaces as separators." << std::endl;
                    exit(EXIT_FAILURE);
                }
            } else {
                if(config_line.find(sep) != std::string::npos) {
                    continue;
                } else {
                    std::cout << "Config file has inconsistent separators. Tried " << sep << std::endl;
                    std::cout << "If you are using \t as a separator make sure you are not accidentally using multiple spaces as separators." << std::endl;
                    exit(EXIT_FAILURE);
                }
            }
        } 
    }
    return(sep);
}
