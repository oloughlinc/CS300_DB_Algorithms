/*
==================================================================================================
Name        :   CSVFileReader.h
Author      :   Craig O'Loughlin
Version     :   1
Date        :   04/17/2022

Description:

    Roll a filestream, iostream, and stringstream into a single object to parse an input file
    piece by piece per function call. The thought behind this was to be able to read data
    while validating instead of reading the entirety of file into memory first. If I were to 
    redo this project I might just use getline(in, line, ',') functionality instead.

==================================================================================================
*/

#ifndef CSVFILEREADER_H
#define CSVFILEREADER_H

#include <string>
#include <fstream>
#include <sstream>

/**
 * @brief A wrapper class for a filestream plus a stringstream. Takes a filepath as constructor argument
 *          and returns comma separated tokens by line with each call for nextToken.
 * 
 * This was supposed to make working with the csv data from main easier but it is at best a wash.
 * Still I must have learned something from writing it.
 * 
 * while (hasLines()) 
 *      nextLine()
 *      while(hasTokens)
 *          nextToken
 */
class CSVFileReader {

    private:

        std::string filePath;
        std::ifstream inFS;
        std::stringstream currentLine;

        char nextChar;
        bool linesAvail;
        bool tokensAvail;
        int currLineNumber;

        void openFileStream();

    public:

        CSVFileReader();
        CSVFileReader(std::string filePath);

        std::string getFilePath() { return filePath; }
        std::string CurrentLineNumber() { return std::to_string(currLineNumber); }
        bool hasLines() { return linesAvail; }
        bool hasTokens() { return tokensAvail; }

        void NextLine();
        std::string NextToken();

        void Reset();
        void CloseFile() { inFS.close(); }
};

#endif