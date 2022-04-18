#include "Course.h"

/**
 * @brief Print course ID, Name followed by newline to console.
 */
void Course::Print() {
    std::cout << ID << ", " << Name << std::endl;
}

/**
 * @brief Print all prerequisites stored in this object to console. Commas separate each object.
 */
void Course::PrintPrereqs() {

    std::cout << "Prerequisites: ";

    if (prereqs.empty()) {
        std::cout << "None" << std::endl;

    } else {
        for (auto iter = prereqs.begin(); iter != prereqs.end(); iter++) {
            std::cout << *iter;
            // if last prereq end here else add a comma
            if (iter == prereqs.end() - 1) {
                std::cout << std::endl;
            } else {
            std::cout << ", ";
            }
        }
    }
}

// convert the input string to all lowercase chars
// see header comments
std::string Course::lowercase(std::string input) {
    std::string lowercaseInput;
    for (auto c : input) lowercaseInput.push_back(std::tolower(c)); //c mean char
    return lowercaseInput;
}

