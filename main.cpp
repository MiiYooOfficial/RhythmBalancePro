#include <iostream>
#include <sstream>
#include <fstream>

#include "Grouping.h"
#include "GeneralFunctions.h"

int main() {
    std::string problemsFilePath, answersFilePath, line;
    int questionNumber = 1;

    // get path to problems file and create path to answers file
    std::ifstream problems = getFile(problemsFilePath);
    answersFilePath = problemsFilePath;
    int i = int(answersFilePath.length()) - 1;
    while (answersFilePath[i] != '.') i--;
    answersFilePath.insert(i, "Solutions");
    std::ofstream answers(answersFilePath);

    if (problems.is_open()) {
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
}
