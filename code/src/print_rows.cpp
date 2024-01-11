#include "header.h"

int main(int argc, char *argv[])
{
    std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();

    /// READING IN ARGUMENTS
    int start_row = std::stoi(argv[1]);
    std::string argument_2; if(argc>2) argument_2 = argv[2]; 
    std::string argument_3 = "Null"; if(argc>3) argument_3 = argv[3]; 

    std::string split_line = "False";
    int end_row = 0;
    if((argument_2 == "True") || (argument_2 == "False")) {
        split_line = argument_2;
    } else { 
        try { end_row = std::stoi(argument_2); }   
        catch(...) {end_row = 0;}
    }
    if((argument_3 == "True") || (argument_3 == "False")) {
        split_line = argument_3;
    } else { 
        try { end_row = std::stoi(argument_3); }  
        catch(...) {end_row = 0;}
    }

    int n_lines = 0;
    std::string line;
    while (std::getline(std::cin, line)) {
        n_lines++;
        if(n_lines >= start_row) {
            if(n_lines < end_row || end_row == 0) {
                std::cout << n_lines << ": " << line << std::endl;
                if(split_line == "True") {
                    std::vector<std::string> line_vec(splitString(line, ','));
                    std::cout << "Split line first and last elements: " << line_vec[0] << " and " << line_vec[line_vec.size()-1] << std::endl;
                }
            }
        } 
        if(n_lines >= end_row && (end_row != 0)) {
            break;
        }
        write_line_update(n_lines, begin);
    }

    write_end_run_summary(begin);
}