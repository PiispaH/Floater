#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>


int find_valid_floater(std::string s, int index) {
    // Finds next valid floater in a string starting from given index
    // int index = s.find("floater");

    std::vector<char> chars = {'(', '{', '[', ']', '}', ')', '\"'};
    std::string order = "";
    int next_floater = s.find("floater");
    bool found;
    int last_char_index = 0; 
    for (int i = index; i < size(s); i++) {
        char character = s[i];
        found = (std::find(chars.begin(), chars.end(), character) != chars.end());
        
        if (found) {
            if ((last_char_index < next_floater) && (next_floater < i)) {
                order += 'X';
                order += character;
                next_floater = -1;
            } else order += character;
        }
    }
    std::cout << order;
    return 0;
}

std::vector<int> find_next_floater(std::string s) {
    std::vector<int> v;
    int index = s.find("floater");
    if (index == -1) {
        v.push_back(-1);
    }
    while (index != -1)
    {
        v.push_back(index);
        index = s.find("floater", index + 1);
    }
    
    return v;
}

std::string convert_floaters(std::string file) {
    std::string s;
    std::ifstream f_in(file);
    std::ofstream f_out("float.py");
    std::vector<int> indexes;

    while (std::getline(f_in, s)) {
        indexes = find_next_floater(s);  // Get the starting indices of the floater -instances
        if (size(indexes) == 1 && indexes[0] == -1) {
            // If no instances of floater is found, keep the row as it is
            f_out << s << std::endl;
        } else {
            int last = -1;  // keep track of the starting index of the last instance of floater in the row
            for (int i : indexes) {  // Go through every starting index
                if (last == -1) {
                    // in the beginning get the whole row from beginning to the start of the floater
                    f_out << s.substr(0, i) + "float";
                } else {
                    // Print from the end of the last floater till the beginning of the next and replace it with float 
                    f_out << s.substr(last + 7, i - last - 7) + "float";
                }
                last = i;
            }
            f_out << s.substr(last + 7) << std::endl;  // Add the rest of the row after the last instance
        }
    }
    f_in.close();
    return "float.py";
}

bool check_only_one_dot(std::string s) {
    int index = s.find(".");
    if (index == -1) return false;
    else if (s.substr(index + 1).find(".") != -1) return false;
    return true;
}

bool check_validity_of_filename(std::string filename) {
    int index = filename.find(".flo");
    int len = size(filename);
    std::ifstream file(filename);
    bool valid = true;

    if (len <= 5) valid = false;  // Filename is longer than just .flo
    else if (!check_only_one_dot(filename)) valid = false;  // a.b.flo not possible
    else if (index != len - 4) valid = false;  // Final 4 characters must be .flo
    else if (!file.good()) valid = false;  // Finally check that the file exists 
    return valid;
}

int main() {
    // int argc, char** argv
    // if (argc == 1) return 1;
    // std::string file = argv[1];
    std::string file = "float.flo";

    if (!check_validity_of_filename(file)) {
        std::cout << "Invalid filename: " << file << std::endl;
        return 1;
    }

    // convert_floaters(file);
    std::string s = "f\"{type()} floaterfloater float er\"";
    find_valid_floater(s, 0);

    /*
    std::string new_file = convert_floaters(file);
    if (file != "") {
        std::cout << "\nConversion completed, float on!" << std::endl;
        std::cout << "======= Program output =======\n\n";
        std::cout << system(new_file.c_str()) << std::endl;
    }
    */
    return 0;
}
