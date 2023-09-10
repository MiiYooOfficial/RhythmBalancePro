#pragma once

// transforms a raw string of notes into a simplified string of notes of the same beats value
// arguments: the raw string of notes
// returns a simplified version of the raw notes as a string
// simplifications include breaking apart dotted notes and representing tuplets using non-tuplet individual notes
std::string processRawNotes(std::string rawNotes);

// transforms a tuplet string of notes into a simplified string of notes of the same beats value
// arguments: the type of tuplet as a string, the tuplet of notes as a string
// returns nothing
// the tuplet string's original content is wiped and replaced with the simplified string of notes
void processTuplet(std::string type, std::string &tuplet);

// calculates the total beats based on the simplified string of notes and the denominator of the time signature the notes belong to
// arguments: the time signature's denominator as an int, the string of simplified notes
// returns the calculated beats as a decimal value in relation to what note gets the beat (i.e., the time signature's denominator)
double computeTotalBeats(int timeSignatureDenominator, std::string simplifiedNotes);