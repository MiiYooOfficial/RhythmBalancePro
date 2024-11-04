#pragma once

class Grouping {
	private:
		std::string rawNotes;
		std::string simplifiedNotes;
		double totalValue;
	public:
		// Constructor
		Grouping(int timeSignatureDenominator, std::string notes);

		// Accessors
		std::string getRawNotes();
		std::string getSimplifiedNotes();
		double getTotalValue();
};

