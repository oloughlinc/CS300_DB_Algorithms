/*
==================================================================================================
Name        :   Course.h
Author      :   Craig O'Loughlin
Version     :   1
Date        :   04/17/2022

Description:

    Object to hold the course information, along with comparison and print functions.

==================================================================================================
*/

#ifndef COURSE_H
#define COURSE_H

#include <vector>
#include <string>
#include <iostream>

/** @brief A handy collection of fields and functions to hold information about a college course.
 */
class Course {

    public:

        // fields
        std::string ID;
        std::string Name;
        std::vector<std::string> prereqs;

        // operator overloads: convert to lowercase and compare by ID
        // i ref https://stackoverflow.com/questions/313970/how-to-convert-an-instance-of-stdstring-to-lower-case
        // "ascii only"
        
        std::string lowercase(std::string input);

        bool operator<(const Course& rhs) {
            return lowercase(this->ID) < lowercase(rhs.ID);
        }
        bool operator==(const Course& rhs) {
            return lowercase(this->ID) == lowercase(rhs.ID);
        }

        // stream overload
        friend std::ostream& operator<<(std::ostream& os, const Course& course) {
            os << course.ID << ", " << course.Name;
            return os;
        }

        // printing functions
        void Print();
        void PrintPrereqs();
};

#endif