#include "../header.h"

int main(int argc, char *argv[]) {
    std::string finreg_path = argv[1];
    std::string res_path = argv[2];

    std::unordered_map<std::string, std::string> finreg_map;
    read_finreg_map(finreg_path, finreg_map);
    write_updated_file(res_path, finreg_map);
}
