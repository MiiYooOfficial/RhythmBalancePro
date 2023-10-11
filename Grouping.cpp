#include <iostream>
#include <string>

#include "Grouping.h"
#include "GroupingFunctions.h"

Grouping::Grouping(int timeSignatureDenominator, std::string notes) {
	rawNotes = notes;
	simplifiedNotes = processRawNotes(rawNotes);
	totalValue = computeTotalBeats(timeSignatureDenominator, simplifiedNotes);
}

std::string Grouping::getRawNotes() {
	return rawNotes;
}

std::string Grouping::getSimplifiedNotes() {
	return simplifiedNotes;
}

double Grouping::getTotalValue() {
	return totalValue;
}