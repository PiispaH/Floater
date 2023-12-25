#include <fstream>
#include <string>
#include <vector>
#include <algorithm>


bool check_only_one_dot(std::string s) {
    // Checks that the given string contains only a one dot (.)
    int index = s.find(".");
    if (index == -1) return false;
    else if (s.substr(index + 1).find(".") != -1) return false;
    return true;
}

bool check_validity_of_filename(std::string filename) {
    // Checks if the given string is a valid .flo file filename or not
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

std::string remove_closed_brackets(std::string s) {
    // Gets rid closed bracet pairs [({}]) -> [(])
    int len = size(s);
    std::string new_s = "";
    char last = ' ';
    for (int i = 0; i < len; i++) {
        if (s[i] == ')' && last == '(') new_s = s.substr(0, i - 1);
        else if (s[i] == '}' && last == '{') new_s = s.substr(0, i - 1);
        else if (s[i] == ']' && last == '[') new_s = s.substr(0, i - 1);
        else new_s += s[i];
        last = s[i];
    }

    if (new_s != s) return remove_closed_brackets(new_s);
    else return new_s;
}

bool convertable(std::string s) {
    // Returns true if the instance is convertable
    int index = s.find("X");
    if (index == 0) return true;
    else if (s[index - 1] == '\"' && s[index + 1] == '\"') return false;
    else if (s[index - 1] == '\'' && s[index + 1] == '\'') return false;
    else return true;
}

int find_valid_floater(std::string s, int index) {
    // Finds next valid floater in a string starting from given index
    std::vector<char> chars = {'(', '{', '[', ']', '}', ')', '\"', '\''};
    std::string order = "";
    int next_floater = s.find("floater", index);
    bool found;
    bool insert_floater = true;
    int last_char_index = 0; 
    for (int i = 0; i < size(s); i++) {
        char character = s[i];
        found = (std::find(chars.begin(), chars.end(), character) != chars.end());
        
        if (found) {
            if (insert_floater && (last_char_index < next_floater) && (next_floater < i)) {
                order += 'X';
                order += character;
                insert_floater = false;
                last_char_index = i;
            } else order += character;
        }
    }
    order = remove_closed_brackets(order);
    if (!convertable(order)) {
        return find_valid_floater(s, next_floater + 7);
    }
    return next_floater;
}

std::vector<int> valid_floaters_for_row(std::string s) {
    std::vector<int> v;
    int index = find_valid_floater(s, 0);
    if (index == -1) {
        v.push_back(-1);
    }
    while (index != -1)
    {
        v.push_back(index);
        index = find_valid_floater(s, index + 1);
    }
    
    return v;
}

std::string convert_floaters(std::string file) {
    // Takes a filename of a .flo file and makes a copy of that
    // into a .py file with every floater converted into
    // float so that the code is runnable in python. 

    std::string s;
    std::ifstream f_in(file);
    std::ofstream f_out("float.py");
    std::vector<int> indexes;

    while (std::getline(f_in, s)) {
        indexes = valid_floaters_for_row(s);  // Get the starting indices of the floater -instances
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
