#pragma once

#include <string>

#include "Grouping.h"

std::string RIA(const int timeSignatureNumerator, const int timeSignatureDenominator, Grouping beginningNotes, Grouping endingNotes);

// completes a single beat using rests based on the notes already present within that beat and the time signature's denominator
// arguments: the denominator of the time signature, the notes already present as an object of the class Grouping
// returns the rests as a string
// the string containing the rests will not necessarily contain the rests in the correct order
std::string completeBeat(const int timeSignatureDenominator, Grouping notes);

// checks if a rest can be constructed from a given value
// arguments: the value of the rest to be constructed as a decimal
// returns true if a rest can be formed from the given rest value and false if not
// only validates rests whose values are equal to or less than a whole rest
// used when completing a single beat
bool validateDoubleAsRest(double restValue);

// creates a rest based on the rest's absolute value and the denominator of the time signature
// arguments: the rest's absolute value, the time signature's denominator
// returns the created rest as a string
std::string createRest(double absoluteValue, const int timeSignatureDenominator);

// re-orders a string of rests so that the rests are in reverse order
// arguments: the original string of rests
// returns nothing
// the raw string of rests is passed by reference as reversal happens in-place
// used when completing a beat at the end portion of a measure
void reverseRests(std::string &rests);

// returns the note with the smallest value based on a note Grouping's simplified series of notes
// arguments: an object of the Grouping class from which the smallest-valued note is to be found
// returns the note with the smallest value as an int (e.g. a sixteenth note is returned as 16)
int findNoteWithSmallestValue(Grouping notes);

std::string completeMeasure(int timeSignatureNumerator, int timeSignatureDenominator, int totalBeats, bool* beatCompleted);
