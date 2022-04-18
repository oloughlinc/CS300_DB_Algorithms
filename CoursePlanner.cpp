/*
==================================================================================================
Name        :   CoursePlanner.cpp
Author      :   Craig O'Loughlin
Version     :   1
Date        :   04/17/2022

Description:

    Console program that loads a given CSV file of course information into memory. Allows
    user to search the course information, print specific course information via a query,
    and print all courses in alphanumeric order by ID.

==================================================================================================
*/

// standard library includes
#include <iostream>
#include <vector>
#include <string>
#include <unordered_map> // local validCourses in validation algorithm
#include <limits> // numeric_limits , for clearing cin
#include <random> // shuffling data objects
#include <algorithm> // std::shuffle

// custom library includes
#include "Course.h"
#include "CSVFileReader.h"
#include "BinarySearchTree.h"

// function declarations
Course newCourse();
std::vector<Course> validate(CSVFileReader& csv);
void checkPrereqs(std::vector<Course> coursesVec, std::unordered_map<std::string, bool>& validCourses);
bool mainMenu();
void PrintCourseList();
void LoadDataStructure();
void buildCourses(std::vector<Course> courses);
void PrintCourse();
void MenuOptions();
void GetInputInt(int& choice);
bool invalidIntInput(int& choice);
std::string getFilePath();

// our chosen data structure
BinarySearchTree<Course> courses;

// entry point
int main() {

    try {
        std::cout << "Welcome to the course planner." << std::endl;
        while(mainMenu()); //main program loop
    } 
    catch (std::exception genError) {
        std::cout << "An unknown error occurred: " << genError.what() << std::endl;
    }
    std::system("pause");
}

// print menu options to console
void MenuOptions() {
    
    std::cout << std::endl;
    std::cout << "1. Load Data Structure." << "\n";
    std::cout << "2. Print Course List." << "\n";
    std::cout << "3. Print Course." << "\n";
    std::cout << "9. Exit" << "\n";
    std::cout << std::endl;

    std::cout << "What would you like to do? ";
}

// main program loop. display options, get choice, route to correct function
bool mainMenu() {

    MenuOptions();
    int choice;
    GetInputInt(choice);

    switch (choice) {

        case 0: 
            std::cout << "Invalid input." << std::endl;
            break;

        case 1: 
            LoadDataStructure();
            break;

        case 2:
            PrintCourseList(); // checks for data before running
            break;

        case 3:
            PrintCourse(); // checks for data before running
            break;

        case 9:
            std::cout << "Thank you for using the course planner!" << std::endl;
            return false; // quit condition

        default:
            std::cout << choice << " is not a valid option." << std::endl;
            break;
    }
    return true; // continue
}

// accept a single int as input otherwise return 0
void GetInputInt(int& choice) {

    if (invalidIntInput(choice)) {
        choice = 0; // set input to zero (will fail switch and loop back here)
        std::cin.clear(); // clear the error
    } 
    //either way, ignore rest of input (should always have a newline left to clear at least)
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); 
}

// I could not decide if it accomplished better readability to make this its own function or not
// or if the extra lines werent worth it
bool invalidIntInput(int& choice) {
    return (!(std::cin >> choice)) or                                    // if getting a single int fails,
            choice < 0             or                                   // or if input is negative
            !( std::cin.peek() == EOF or std::cin.peek() == '\n');      // or if the next char is not newline or end of line
}

// get the file path from user, parse by comma, validate, and build the data structure
void LoadDataStructure() {

    try {
        auto csv = CSVFileReader(getFilePath()); // throws runtime errors
        buildCourses(validate(csv)); // throws also runtime errors
        std::cout << csv.getFilePath() << " loaded successfully!" << std::endl;
        csv.CloseFile();
    }
    catch (std::runtime_error e) { // the errors describe where the function failed
        std::cout << e.what() << std::endl;
    }
}

// get a file path from the user as a string with no restrictions
std::string getFilePath() {
    std::string filePath;
    std::cout << "Please enter file name with extension: ";
    getline(std::cin, filePath);
    return filePath;
}

// populates the data structure with course objects from the validated file
void buildCourses(std::vector<Course> coursesVec) {

    courses.Clear(); //do we want to clear or keeping adding more files??

    //shuffle the input as it typically leads to better tree balance in presorted inputs
    auto random = std::default_random_engine {};
    std::shuffle(coursesVec.begin(), coursesVec.end(), random); //linear time

    // n                                    log n
    for (auto course : coursesVec) courses.Insert(course);
}

// print everything in the data structure
void PrintCourseList() {

    if (! courses.isEmpty()) {
        std::cout << "Here is a sample schedule:\n" << std::endl;

        // all that iterator code in the tree just for this
        for (auto course : courses) course.Print();
    }
    else {
        std::cout << "No courses to display. Please load courses first." << std::endl;
    }
}

// find a specific course and print its information if available
void PrintCourse() {

    if (! courses.isEmpty()) {
        Course search;
        std::cout << "What course do you want to know about? ";
        getline(std::cin, search.ID);

        /* Return a copy of the object instance in tree
        if found. if not found, return the search object. Use a blank search object with 
        just the identifying criteria filled, in this case search.ID
        */
        search = courses.Search(search); // returns course if found or search if not
        
        if (search.Name != "") { // if a search was found, name would be filled on the object
            search.Print();
            search.PrintPrereqs();
        } else {
            std::cout << search.ID << " not found." << std::endl;
        }
    }
    else {
        std::cout << "No courses to search for. Please load courses first." << std::endl;
    }
}

// build course objects from file while ensuring that parameters are met in file:
// 1. at least two values exist on each line (comma seperated)
// 2. any prerequisites exists as a course (first token of each line) somewhere in the file
// 3. No empty values
std::vector<Course> validate(CSVFileReader& csv) {

    std::vector<Course> coursesVec;
    std::unordered_map<std::string, bool> validCourses;

    while (csv.hasLines()) { // iterate each line of file
        csv.NextLine();
        if (!csv.hasTokens()) continue; //skip empty line

        Course newCourse;

        // first token
        newCourse.ID = csv.NextToken();
        validCourses.emplace(newCourse.ID, true);

        // second token does not exist?
        if (!csv.hasTokens()) throw std::runtime_error("Error in input file (line " + csv.CurrentLineNumber() +"): Not enough values in line.");

        // second token
        newCourse.Name = csv.NextToken();

        // -> n remaining tokens to prereqs
        while (csv.hasTokens()) newCourse.prereqs.push_back(csv.NextToken());

        // then add the course to the list
        coursesVec.push_back(newCourse);
    }
    checkPrereqs(coursesVec, validCourses); // throws error if invalid

    return coursesVec; //if no errors program reaches the return with a validated array of objects ready for insertion
    // i suppose there can still be nonsense data ("ID: asdasdia") in the objects but is there a good way to validate that?
}

void checkPrereqs(std::vector<Course> coursesVec, std::unordered_map<std::string, bool>& validCourses) {

    // check each prereq against valid course map
    // map makes this O(n) for prereqs amount

    int line = 0; //track which line an error may occur. each course object is built from a single line
    for (auto course : coursesVec) {
        line++;
        for (auto prereq : course.prereqs) {
            if (validCourses.find(prereq) == validCourses.end()) { //if prereq not a valid course
                throw std::runtime_error("Error in input file (line " + 
                        std::to_string(line) + "): One or more prerequisites do not exist as a course.");
            }
        } //also check for empty values while iterating
        if (course.ID == "" or course.Name == "") throw std::runtime_error(
                                "Error in input file (line " + std::to_string(line) + "): Empty value.");
    }
}