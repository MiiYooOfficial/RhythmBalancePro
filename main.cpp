#include <iostream>
#include <string>
#include <sstream>
#include <fstream>
#include <algorithm>

#include "Globals.h"
#include "Grouping.h"
#include "GeneralFunctions.h"

int main() {
    // get path to problems file
    std::string problemsFilePath;
    std::ifstream problems = getFile(problemsFilePath);

    // determine if the problems file is a text file or a MusicXML file
    std::string problemFileType;
    int problemFileTypeOption;
    for (int i = int(problemsFilePath.length()) - 1; problemsFilePath[i] != '.'; i--)
        problemFileType.push_back(problemsFilePath[i]);
    std::reverse(problemFileType.begin(), problemFileType.end());

    if (problemFileType == "txt")
        problemFileTypeOption = TXT;
    else if (problemFileType == "musicxml")
        problemFileTypeOption = MUSICXML;
    else {
        std::cout << "ERROR: Unknown file type" << std::endl;
        return 0;
    }

    // create solutions file
    std::string solutionsFilePath = problemsFilePath;
    int i = int(solutionsFilePath.length()) - 1;
    while (solutionsFilePath[i] != '.') i--;
    solutionsFilePath.insert(i, "_solutions");
    std::ofstream answers(solutionsFilePath);

    if (problemFileTypeOption == TXT) {
        int questionNumber = 1;

        if (problems.is_open()) {
            std::string line;
            while (std::getline(problems, line)) {
                int lineIndex = 0;
                int timeSignatureNumerator = 0, timeSignatureDenominator = 0;

                while (!(line[lineIndex] == '|' && line[lineIndex + 1] == '|' && lineIndex + 1 == line.length() - 1)) {
                    std::string question;

                    if (line[lineIndex] == '|' && line[lineIndex + 1] == '|') // problem has new time signature
                        setTimeSignature(line, lineIndex, timeSignatureNumerator, timeSignatureDenominator);
                    else // problem has same time signature
                        lineIndex++;

                    while (line[lineIndex] != '|') {
                        question.push_back(line[lineIndex]);
                        lineIndex++;
                    }

                    answers << "Question " << questionNumber << ": ";
                    if (validateTimeSignature(timeSignatureNumerator, timeSignatureDenominator))
                        answers << solveQuestion(timeSignatureNumerator, timeSignatureDenominator, question) << "\n";
                    else
                        answers << "invalid time signature\n";

                    questionNumber++;
                }
            }
        }

        answers.close();
        std::cout << "\n" << "Homework Solved!" << std::endl << std::endl;
    }

    else if (problemFileTypeOption == MUSICXML) {
        // convert problems file to array of strings
        int numLines = 0;
        std::string* file = new std::string[numLines];
        std::string line;
        if (problems.is_open()) {
            while (!problems.eof()) {
                std::getline(problems, line);
                file = addElementToArray(file, numLines, line, numLines);
            }
        }

        // solve problems
        int divisions, timeSignatureNumerator, timeSignatureDenominator;
        std::string question;
        int questionNumber;
        bool bracketFound = false;
        for (int problemsFileIndex = 0; problemsFileIndex < numLines; problemsFileIndex++) {
            std::string currentLine = file[problemsFileIndex];

            if (currentLine.find("measure number") != std::string::npos) {
                int lineIndex = 0;
                std::string measureNumberAsString;

                while (currentLine[lineIndex] < 48 || currentLine[lineIndex] > 57)
                    lineIndex++;
                while (currentLine[lineIndex] >= 48 && currentLine[lineIndex] <= 57) {
                    measureNumberAsString.push_back(currentLine[lineIndex]);
                    lineIndex++;
                }

                questionNumber = stoi(measureNumberAsString);
            }
            else if (currentLine.find("divisions") != std::string::npos || currentLine.find("beats") != std::string::npos || currentLine.find("beat-type") != std::string::npos) {
                int lineIndex = 0;
                std::string elementAsString;
                while (currentLine[lineIndex] < 48 || currentLine[lineIndex] > 57)
                    lineIndex++;
                while (currentLine[lineIndex] >= 48 && currentLine[lineIndex] <= 57) {
                    elementAsString.push_back(currentLine[lineIndex]);
                    lineIndex++;
                }

                if (currentLine.find("divisions") != std::string::npos) // set divisions variable from xml file
                    divisions = stoi(elementAsString);
                else if (currentLine.find("beats") != std::string::npos) // set numerator of time signature
                    timeSignatureNumerator = stoi(elementAsString);
                else if (currentLine.find("beat-type") != std::string::npos) // set denominator of time signature
                    timeSignatureDenominator = stoi(elementAsString);
            }
            else if (currentLine.find("<type>") != std::string::npos) { // add note to question
                std::string noteValueAsString;
                int lineIndex = 0;
                while (currentLine[lineIndex] != '>')
                    lineIndex++;
                lineIndex++;
                while (currentLine[lineIndex] != '<') {
                    noteValueAsString.push_back(currentLine[lineIndex]);
                    lineIndex++;
                }

                question.append(convertNote(noteValueAsString) + " ");
            }
            else if (currentLine.find("dot") != std::string::npos) { // add dot to associated note
                question[question.length() - 1] = '*';
                question.push_back(' ');
            }
            else if (currentLine.find("<time-modification>") != std::string::npos) { // process tuplet
                std::string tupletTypeAsString;
                int tupletType;
                int tempFileIndex = problemsFileIndex;

                // determine type of tuplet
                while (!(file[tempFileIndex].find("<actual-notes>") != std::string::npos))
                    tempFileIndex++;
                int lineIndex = 0;
                while ((file[tempFileIndex])[lineIndex] < 48 || (file[tempFileIndex])[lineIndex] > 57)
                    lineIndex++;
                while ((file[tempFileIndex])[lineIndex] >= 48 && (file[tempFileIndex])[lineIndex] <= 57) {
                    tupletTypeAsString.push_back((file[tempFileIndex])[lineIndex]);
                    lineIndex++;
                }
                tupletType = stoi(tupletTypeAsString);
                tempFileIndex = problemsFileIndex;

                // determine if this is the first or last note of tuplet (or neither)
                while (!(file[tempFileIndex].find("</note>") != std::string::npos)) {
                    if ((file[tempFileIndex]).find("tuplet type") != std::string::npos) {
                        int questionIndex = question.length() - 1;

                        if ((file[tempFileIndex]).find("start") != std::string::npos) { // start of tuplet
                            questionIndex--; // remove space at end of question
                            while (question[questionIndex] != ' ' && questionIndex != 0)
                                questionIndex--;
                            question.insert(questionIndex, " " + convertTupletIntToString(tupletType) + "-");
                        }
                        else if ((file[tempFileIndex]).find("stop") != std::string::npos) // end of tuplet
                            question.insert(questionIndex, "-" + convertTupletIntToString(tupletType));

                        break;
                    }

                    tempFileIndex++;
                }
            }
            else if (currentLine.find("breath-mark") != std::string::npos) { // insert bracket into question
                question.append("b ");
                bracketFound = true;
            }
            else if (currentLine.find("/measure") != std::string::npos) { // end of question
                if (!bracketFound) // answer bracket must be found at start of question
                    question.insert(0, "b ");

                // remove any spaces at start and end of question
                while (question[0] == ' ')
                    question.erase(0, 1);
                while (question[question.length() - 1] == ' ')
                    question.pop_back();

                file = addAnswerToFileArray(file, numLines, solveQuestion(timeSignatureNumerator, timeSignatureDenominator, question), questionNumber, divisions);

                question.clear();
                bracketFound = false;
                questionNumber++;

                // find end of question
                while (!(file[problemsFileIndex].find("/measure") != std::string::npos))
                    problemsFileIndex++;
            }
        }

        // remove breath marks
        for (int fileIndex = 0; fileIndex < numLines; fileIndex++) {
            if (file[fileIndex].find("<articulations>") != std::string::npos) {
                for (int i = 0; i < 3; i++)
                    file = removeElementFromArray(file, numLines, fileIndex);

                if (file[fileIndex - 1].find("<notations>") != std::string::npos) { // notations block contains no other information
                    file = removeElementFromArray(file, numLines, fileIndex);
                    file = removeElementFromArray(file, numLines, fileIndex - 1);
                }
            }
        }

        // add program name header
        bool headerInserted = false;
        for (int fileIndex = 0; fileIndex < numLines; fileIndex++) {
            if (file[fileIndex].find("[") != std::string::npos) {
                std::string beginningLine, endingLine;
                int lineIndex = 0;

                while ((file[fileIndex])[lineIndex] != '[') {
                    beginningLine.push_back((file[fileIndex])[lineIndex]);
                    lineIndex++;
                }
                while ((file[fileIndex])[lineIndex] != ']')
                    lineIndex++;
                lineIndex++;
                while (lineIndex < (file[fileIndex]).length()) {
                    endingLine.push_back((file[fileIndex])[lineIndex]);
                    lineIndex++;
                }

                file = removeElementFromArray(file, numLines, fileIndex);
                file = addElementToArray(file, numLines, beginningLine + "[Powered by RhythmBalancePro]" + endingLine, fileIndex);
                headerInserted = true;
            }
        }
        if (!headerInserted) {
            int fileIndex = 0;
            while (!(file[fileIndex].find("<score-partwise") != std::string::npos))
                fileIndex++;
            file = addElementToArray(file, numLines, "<movement-title>[Powered by RhythmBalancePro]</movement-title>", fileIndex + 1);
        }

        // write results to answers XML file
        for (int line = 0; line < numLines; line++)
            answers << file[line] << std::endl;

        std::cout << "\n" << "Homework Solved!" << std::endl << std::endl;
    }

    exitProgram();
}
