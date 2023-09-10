#include <iostream>
#include <string>
#include <fstream>
#include <sstream>

#include "Globals.h"
#include "GeneralFunctions.h"
#include "Grouping.h"
#include "RIA.h"

std::string convertDoubleToSymbol(double value) {
	int symbol = 1;

	while (double(1 / double(symbol)) != value)
		symbol *= 2;

	return std::to_string(symbol);
}

std::ifstream getFile(std::string &filePath) {
    std::cout << "Drag and drop the text file containing the problems to solve and hit Enter: ";
    std::string path;
    std::ifstream file;
    std::getline(std::cin, path);

    // path processing
    if (path[0] == '"' && path[path.length() - 1] == '"')
        path = path.substr(1, path.length() - 2);

    file.open(path);
    while (file.fail()) {
        file.clear();

        std::cout << "File not found, please try again: ";
        std::getline(std::cin, path);
        if (path[0] == '"' && path[path.length() - 1] == '"')
            path = path.substr(1, path.length() - 2);
        
        file.open(path);
    }

    filePath = path;
    return file;
}

void setTimeSignature(const std::string line, int &lineIndex, int &numerator, int &denominator) {
    std::string numeratorAsString, denominatorAsString;

    while (line[lineIndex] < '1' || line[lineIndex] > '9')
        lineIndex++;

    while (line[lineIndex] != '/') {
        numeratorAsString.push_back(line[lineIndex]);
        lineIndex++;
    }
    numerator = stoi(numeratorAsString);

    lineIndex++;
    while (line[lineIndex] != ' ') {
        denominatorAsString.push_back(line[lineIndex]);
        lineIndex++;
    }
    denominator = stoi(denominatorAsString);
}

bool validateTimeSignature(int numerator, int denominator) {
    // check if numerator is invalid
    if (numerator == 8 || numerator > 12)
        return false;
    
    // check if denominator is invalid
    int tester = 1;
    while (tester < denominator)
        tester *= 2;
    if (tester != denominator)
        return false;

    return true;
}

std::string solveQuestion(const int timeSignatureNumerator, const int timeSignatureDenominator, std::string question) {
    std::string beginningNotes, endingNotes;

    // break up question into notes before and after the bracket
    // std::cout << question << std::endl; // debugging
    int questionIndex = 0;
    while (question[questionIndex] == ' ')
        questionIndex++;
    for (; question[questionIndex] != BRACKET; questionIndex++)
        beginningNotes.push_back(question[questionIndex]);
    questionIndex++;
    while (question[questionIndex] == ' ')
        questionIndex++;
    for (; questionIndex != question.length(); questionIndex++)
        endingNotes.push_back(question[questionIndex]);
    while (question[question.length() - 1] == ' ')
        question.pop_back();

    // create groupings for notes before and after the bracket
    Grouping beginning(timeSignatureDenominator, beginningNotes);
    Grouping ending(timeSignatureDenominator, endingNotes);

    return RIA(timeSignatureNumerator, timeSignatureDenominator, beginning, ending);
}