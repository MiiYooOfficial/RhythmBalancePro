#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <algorithm>

#include "Globals.h"
#include "GeneralFunctions.h"
#include "Grouping.h"
#include "RIA.h"

std::ifstream getFile(std::string &filePath) {
    std::cout << "-----------------------------------------------------" << std::endl;
    std::cout << "|                                                   |" << std::endl;
    std::cout << "|                        |||                        |" << std::endl;
    std::cout << "|                        |||                        |" << std::endl;
    std::cout << "|                      \\ ||| /                      |" << std::endl;
    std::cout << "|                       \\   /                       |" << std::endl;
    std::cout << "|                |       \\ /       |                |" << std::endl;
    std::cout << "|                |        v        |                |" << std::endl;
    std::cout << "|                |_________________|                |" << std::endl;
    std::cout << "|                                                   |" << std::endl;
    std::cout << "|            Drag & Drop to Upload File             |" << std::endl;
    std::cout << "|                                                   |" << std::endl;
    std::cout << "-----------------------------------------------------" << std::endl;
    std::cout << std::endl << "Path to file (press Enter to validate): ";
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

void setTimeSignature(const std::string line, int& lineIndex, int& numerator, int& denominator) {
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

std::string convertDoubleToSymbol(double value) {
    int symbol = 1;

    while (double(1 / double(symbol)) != value)
        symbol *= 2;

    return std::to_string(symbol);
}

std::string* addElementToArray(std::string* array, int &capacity, std::string line, int elementNumber) {
    if (elementNumber > capacity)
        return array;

    std::string* temp = new std::string[capacity + 1];
    bool elementInserted = false;
    if (capacity > 0) {
        for (int i = 0; i < capacity + 1; i++) {
            if (i == elementNumber) {
                temp[i] = line;
                elementInserted = true;
            }
            else {
                if (!elementInserted)
                    temp[i] = array[i];
                else
                    temp[i] = array[i - 1];
            }
        }
    }
    else // array is currently empty
        temp[0] = line;

    delete[] array;
    capacity++;
    return temp;
}

std::string* removeElementFromArray(std::string* array, int &capacity, int elementNumber) {
    if (elementNumber > capacity)
        return array;

    std::string* temp = new std::string[capacity - 1];
    bool elementFound = false;
    for (int i = 0; i < capacity; i++) {
        if (i == elementNumber)
            elementFound = true;
        else {
            if (!elementFound)
                temp[i] = array[i];
            else
                temp[i - 1] = array[i];
        }
    }

    delete[] array;
    capacity--;
    return temp;
}

std::string convertNote(std::string noteValueAsString) {
    if (noteValueAsString == "whole")
        return "1";
    else if (noteValueAsString == "half")
        return "2";
    else if (noteValueAsString == "quarter")
        return "4";
    else if (noteValueAsString == "eighth")
        return "8";
    else {
        std::string temp;
        for (int i = 0; noteValueAsString[i] >= 48 && noteValueAsString[i] <= 57; i++)
            temp.push_back(noteValueAsString[i]);

        return temp;
    }
}

std::string convertTupletIntToString(int tupletType) {
    switch (tupletType) {
    case 2:
        return "d";
    case 3:
        return "t";
    case 4:
        return "qa";
    case 5:
        return "qi";
    case 6:
        return "sx";
    case 7:
        return "sp";
    }
}

std::string* addAnswerToFileArray(std::string* file, int &numLines, std::string solution, int questionNumber, int divisions) {
    int lineNumber = 0;
    int startOfMeasure;
    bool answerAdded = false;

    // find start of measure
    while (!(file[lineNumber].find("measure number=\"" + std::to_string(questionNumber) + "\"") != std::string::npos))
        lineNumber++;
    startOfMeasure = lineNumber;

    int solutionLength = 0;
    std::string* solutionBrokenUp = new std::string[solutionLength];
    std::string singleSolution;

    // break up solution into array of individual rests
    for (int i = 0; i < solution.length(); i++) {
        if (solution[i] == ' ') {
            solutionBrokenUp = addElementToArray(solutionBrokenUp, solutionLength, singleSolution, solutionLength);
            singleSolution.clear();
        }
        else
            singleSolution.push_back(solution[i]);
    }
    if (singleSolution.length() > 0)
        solutionBrokenUp = addElementToArray(solutionBrokenUp, solutionLength, singleSolution, solutionLength);

    // add solution to file array
    while (!(file[lineNumber].find("/measure") != std::string::npos)) {
        if (file[lineNumber].find("breath-mark") != std::string::npos) { // insert answer at breath mark
            while (!(file[lineNumber].find("</note>") != std::string::npos))
                lineNumber++;
            lineNumber++;
            
            for (int solutionIndex = 0; solutionIndex < solutionLength; solutionIndex++) {
                file = insertRestToFileArray(file, numLines, lineNumber, solutionBrokenUp[solutionIndex], divisions);
                lineNumber++;
            }

            answerAdded = true;
            break;
        }
        lineNumber++;
    }

    if (!answerAdded) { // insert answer at start of measure
        lineNumber = startOfMeasure + 1;
        while (!(file[lineNumber].find("<note>") != std::string::npos))
            lineNumber++;
        for (int solutionIndex = 0; solutionIndex < solutionLength; solutionIndex++) {
            file = insertRestToFileArray(file, numLines, lineNumber, solutionBrokenUp[solutionIndex], divisions);
            lineNumber++;
        }
    }

    return file;
}

std::string* insertRestToFileArray(std::string* file, int &numLines, int lineNumber, std::string singleSolution, int divisions) {
    std::string lineToAdd;
    int singleSolutionIndex = 0;

    lineToAdd.append("<note><rest/><duration>" + computeDuration(singleSolution, divisions) + "</duration><voice>1</voice><type>" + convertRestToXML(singleSolution) + "</type>");
    while (singleSolutionIndex < (singleSolution).length() && singleSolution[singleSolutionIndex] != '*')
        singleSolutionIndex++;
    while (singleSolutionIndex < (singleSolution).length() && singleSolution[singleSolutionIndex] == '*') {
        lineToAdd.append("<dot/>");
        singleSolutionIndex++;
    }
    lineToAdd.append("</note>");

    return addElementToArray(file, numLines, lineToAdd, lineNumber);
}

std::string computeDuration(std::string rest, int divisions) {
    double duration = 0;
    int restIndex = 0;

    std::string primaryRest;
    while (rest[restIndex] >= 48 && rest[restIndex] <= 57) { // parse primary rest
        primaryRest.push_back(rest[restIndex]);
        restIndex++;
    }
    duration += divisions * (4 / stod(primaryRest));

    int dotNumber = 1;
    while (restIndex < rest.length() && rest[restIndex] == '*') { // parse ensuing dots
        duration += divisions * (4 / stod(primaryRest)) * (0.5 / dotNumber);
        restIndex++;
        dotNumber++;
    }

    return std::to_string(int(duration));
}

std::string convertRestToXML(std::string rest) {
    switch (stoi(rest)) {
        case 11:
            return "breve";
        case 1:
            return "whole";
        case 2:
            return "half";
        case 4:
            return "quarter";
        case 8:
            return "eighth";
        case 16:
            return "16th";
        case 32:
            return "32nd";
        case 64:
            return "64th";
        case 128:
            return "128th";
        case 256:
            return "256th";
        default:
            return ERROR;
    }
}

void exitProgram() {
    std::cout << "Program finished, hit enter to exit . . . ";
    std::cin.ignore();
}
