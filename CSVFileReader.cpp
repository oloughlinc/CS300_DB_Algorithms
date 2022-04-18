#include "CSVFileReader.h"

/* Fun fact, I lost the original code for this on Friday night while doing a cleanup of my c drive :(
*/

// constructor. open filepath and initalize line count
CSVFileReader::CSVFileReader(std::string filePath) {

    this->filePath = filePath;
    openFileStream();
    linesAvail = true;
    currLineNumber = 0;

}

// open the filestream at the designated filepath. throw error if there was an issue.
void CSVFileReader::openFileStream() {
    inFS.open(filePath);
    if (! inFS.is_open()) throw std::runtime_error("Error opening file: " + filePath);
    if (inFS.peek() == EOF) throw std::runtime_error("Error: Empty file.");
}

/**
 * @brief Point the parser state at the next available line in the file. If no more lines, 
 * set the hasLines state to false.
 */
void CSVFileReader::NextLine() {

    std::string line;
    if (! inFS.eof()) {

        getline(inFS, line);
        currLineNumber++;    
        currentLine = std::stringstream(line);
        tokensAvail = (line == "") ? false : true; //empty lines dont have tokens
    }

    if (inFS.eof()) {  //since it can now be the eof after reading the last line, check again
        linesAvail = false;
    }
}

/**
 * @brief Get the next token at the current line up to a comma or the end of the line.
 * If at the end of the line, set hasTokens state to false.
 * 
 * @return C++ string copy of the next token in the file.
 */
std::string CSVFileReader::NextToken() {

    /* on each loop run get the character. by default add that character to the
     token. if the character is ',' or EOL, return the token.

     The place in the currentLine is saved in the object state for the next call.
    */

    std::string token;
    while (! currentLine.eof()) {
        nextChar = currentLine.get();

        switch (nextChar) {
            case EOF:
                tokensAvail = false; // intentional fall thru
            case ',':
                return token;
            default:
                token.push_back(nextChar);
                break;
        }
    }
    return token; // case: ""
}

// point the parser back to the top of the file if needed
void CSVFileReader::Reset() {
    inFS.seekg(0);
    currLineNumber = 0;
    linesAvail = true;
}
