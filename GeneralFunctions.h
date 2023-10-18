#pragma once

#include <string>

// obtains from the user the path to the text file containing the questions to be solved
// arguments: an empty string passed by reference that will contain the name of the file
// returns the input stream associated with the inputted text file
// input parsing allows the questions file to be drag and dropped into the console
std::ifstream getFile(std::string &filePath);

// sets the numerator and denominator values of a time signature to their new values based on a given line
// arguments: the line of notes as a string, the index associated to the line of notes,
// the numerator of the time signature, the denominator of the time signature
// returns nothing
// the line index is passed as reference so future parsing of the line of notes starts where the time signature ends
void setTimeSignature(const std::string line, int& lineIndex, int& numerator, int& denominator);

// validates a given time signature for ensuing problem-solving
// arguments: the time signature's numerator, the time signature's denominator
// returns true if the given time signature is recognized and false if not
bool validateTimeSignature(int numerator, int denominator);

// determines the answer for a particular question
// arguments: the time signature's numerator, the time signature's denominator, the question as a string
// returns the answer as a string
std::string solveQuestion(const int timeSignatureNumerator, const int timeSignatureDenominator, std::string question);

// returns a symbol (for either a note or a rest) as an integral string based on the note's value
// arguments: the symbol's absolute value (i.e., its value in relation to a whole note)
// returns the integral string representing the symbol
std::string convertDoubleToSymbol(double value);

// updates an array of strings by increasing the size by one and including the newest string at the desired location
// arguments: the array as it currently is, the current capacity, the string to be included, the location at which the string should be inserted
// returns the updated array
// takes care of the old array upon execution
// the array's capacity is increased by one
std::string* addElementToArray(std::string* array, int &capacity, std::string newLine, int elementNumber);

// updates an array of strings by decreasing the size by one and omitting an element as specified in the parameters
// arguments: the array as it currently is, the current capacity, the index of the element to be removed
// returns the updated array
// takes care of the old array upon execution
// the array's capacity is decreased by one
std::string* removeElementFromArray(std::string* array, int &capacity, int elementNumber);

// converts a note in MusicXML format to the program's own notation format
// arguments: the note value as a string in MusicXML format
// returns the note value according to the program's music notation format
std::string convertNote(std::string noteValueAsString);

// converts a tuplet type as an integer to a string according to the program's notation system
// arguments: the type of tuplet as an integer
// returns the tuplet type as a string according to the program's own notation system for tuplets
std::string convertTupletIntToString(int tupletType);

// inserts the answer to a single problem at the desired location in the XML file as represented by an array of strings
// arguments: the file as an array of strings, the number of lines the file array currently has, the solution to the problem in question,
// the question number, the divisions constant as set acccording to the original XML file
// returns the updated file as an array of strings with the solution to the original problem added at the correct location in XML notation format
// the total number of lines is increased by the number of lines that were inserted based on the problem's solution
std::string* addAnswerToFileArray(std::string* file, int &numLines, std::string solution, int questionNumber, int divisions);

// adds a single rest in XML notation format as a single line to an XML file represented by an array of strings
// arguments: the file as an array of strings, the number of lines the file array currently has, the line number at which you wish to insert the rest,
// the rest you wish to insert (in the program's own notation format), the divisions constant as set according to the orignal XML file
// returns the file with the newly added line (representing the inserted rest in XML notation format)
// the total number of lines is increased by one
std::string* insertRestToFileArray(std::string* file, int &numLines, int lineNumber, std::string singleSolution, int divisions);

// determines the total duration of a rest based on the divisions constant of the XML problems file
// arguments: the rest whose duration you wish to compute, the divisions constant set according to the original XML file
// returns the duration of the rest as a string
std::string computeDuration(std::string rest, int divisions);

// converts a rest in the program's own notation format according to XML's notation format
// arguments: the rest whose XML notation you wish to determine
// returns the rest in XML format as a string
std::string convertRestToXML(std::string rest);