#include <iostream>
#include <string>

#include "Grouping.h"
#include "GroupingFunctions.h"

Grouping::Grouping(int timeSignatureDenominator, std::string notes) {
	rawNotes = notes;
	simplifiedNotes = processRawNotes(rawNotes);
	totalValue = computeTotalBeats(timeSignatureDenominator, simplifiedNotes);

	// debugging
	/*
	std::cout << "---GROUPING START---" << std::endl;
	std::cout << "Raw Notes: " << rawNotes << std::endl;
	std::cout << "Simplified Notes: " << simplifiedNotes << std::endl;
	std::cout << "Total Value: " << totalValue << std::endl;
	std::cout << "---GROUPING END---" << std::endl;
	*/
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