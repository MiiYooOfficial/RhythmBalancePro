#pragma once

#include <string>

// returns a symbol (for either a note or a rest) as an integral string based on the note's value
// arguments: the symbol's pure value (i.e., its value in relation to a whole note)
// returns the integral string representing the symbol
std::string convertDoubleToSymbol(double value);

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
void setTimeSignature(const std::string line, int &lineIndex, int &numerator, int &denominator);

// validates a given time signature for ensuing problem-solving
// arguments: the time signature's numerator, the time signature's denominator
// returns true if the given time signature is recognized and false if not
bool validateTimeSignature(int numerator, int denominator);

// determines the answer for a particular question
// arguments: the time signature's numerator, the time signature's denominator, the question as a string
// returns the answer as a string
std::string solveQuestion(const int timeSignatureNumerator, const int timeSignatureDenominator, std::string question);