#include <iostream>
#include <cstdio>
#include <memory>
#include <stdexcept>
#include <array>

#include "src\handle_strings.hpp"


std::string exec(const char* cmd) {
    std::array<char, 128> buffer;
    std::string result;
    std::unique_ptr<FILE, decltype(&_pclose)> pipe(_popen(cmd, "r"), _pclose);
    if (!pipe) {
        throw std::runtime_error("_popen() failed!");
    }
    while (fgets(buffer.data(), buffer.size(), pipe.get()) != nullptr) {
        result += buffer.data();
    }
    return result;
}

int main(int argc, char** argv) {
    if (argc == 1) return 1;
    std::string file = argv[1];

    if (!check_validity_of_filename(file)) {
        std::cout << "Invalid filename: " << file << std::endl;
        return 1;
    }

    std::string new_file = convert_floaters(file);
    std::cout << "\nConversion completed, float on!" << std::endl;
    std::cout << "======= Program output =======\n\n";
    std::string result = exec(new_file.c_str());
    std::cout << result << std::endl;
    return 0;
}
