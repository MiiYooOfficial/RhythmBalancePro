#include <string>

#include "GroupingFunctions.h"
#include "Globals.h"
#include "GeneralFunctions.h"

std::string processRawNotes(std::string rawNotes) {
	std::string rawNotesProcessed = "";

	for (int rawNotesIndex = 0; rawNotesIndex < rawNotes.length(); rawNotesIndex++) {
		if (rawNotes[rawNotesIndex] == ' ') // accept space
			rawNotesProcessed.push_back(' ');
		else if (rawNotes[rawNotesIndex] > '0' && rawNotes[rawNotesIndex] < ':') // character is a note
			rawNotesProcessed.push_back(rawNotes[rawNotesIndex]);
		else if (rawNotes[rawNotesIndex] == '*') { // break up dotted notes
			int numberOfHalves = 1;
			std::string associatedNote;
			std::string noteToAppend;

			int reverseIterator;
			for (reverseIterator = rawNotesIndex - 1; rawNotes[reverseIterator] == '*'; reverseIterator--) // find how many dots precede this dot
				numberOfHalves++;
			for (; reverseIterator >= 0 && rawNotes[reverseIterator] >= '1' && rawNotes[reverseIterator] <= '9'; reverseIterator--)
				associatedNote.push_back(rawNotes[reverseIterator]);
			std::reverse(associatedNote.begin(), associatedNote.end());

			noteToAppend = associatedNote;
			for (int halvingIterator = 0; halvingIterator < numberOfHalves; halvingIterator++) // determine note value of dot
				noteToAppend = std::to_string(stoi(noteToAppend) * 2);

			rawNotesProcessed.append(" " + noteToAppend);
		}
		else if (rawNotes[rawNotesIndex] == 'd' || rawNotes[rawNotesIndex] == 't' || rawNotes[rawNotesIndex] == 'q' || rawNotes[rawNotesIndex] == 's') { // start of a tuplet
			std::string notesWithinTuplet;
			std::string tupletType;
			int tupletIndex = rawNotesIndex;

			while (rawNotes[tupletIndex] != '-') { // find first note of tuplet
				tupletType.push_back(rawNotes[tupletIndex]);
				tupletIndex++;
			}
			tupletIndex++;

			while (rawNotes[tupletIndex] != '-') { // collect all notes within tuplet
				notesWithinTuplet.push_back(rawNotes[tupletIndex]);
				tupletIndex++;
			}

			notesWithinTuplet = processRawNotes(notesWithinTuplet);
			processTuplet(tupletType, notesWithinTuplet);
			rawNotesProcessed.append(notesWithinTuplet);

			// find new position for rawNotesIndex by using tupletIndex
			tupletIndex++;
			while (rawNotes[tupletIndex] >= 97 && rawNotes[tupletIndex] <= 122)
				tupletIndex++;
			rawNotesIndex = tupletIndex - 1;
		}
		else { // unrecognized character
			return ERROR;
		}
	}

	return rawNotesProcessed;
}

void processTuplet(std::string type, std::string &tuplet) {
	double tupletValue = 0;
	std::string note;

	// determine raw value of notes within tuplet
	for (int i = 0; i < tuplet.length(); i++) {
		if (tuplet[i] >= '1' && tuplet[i] <= '9') {
			std::string note;
			int j;
			for (j = i; tuplet[j] >= '1' && tuplet[j] <= '9'; j++)
				note.push_back(tuplet[j]);

			tupletValue += 1 / stod(note);
			i = j;
		}
	}
	
	// determine singular note tuplet's equivalent can be built from
	if (type[0] == 'd') {
		tupletValue /= 2;
		note = convertDoubleToSymbol(tupletValue);
	}
	else if (type[0] == 't') {
		tupletValue /= 3;
		note = convertDoubleToSymbol(tupletValue);
	}
	else if (type[0] == 'q') {
		if (type[1] == 'a') {
			tupletValue /= 4;
			note = convertDoubleToSymbol(tupletValue);
		}
		else if (type[1] == 'i') {
			tupletValue /= 5;
			note = convertDoubleToSymbol(tupletValue);
		}
	}
	else if (type[0] == 's') {
		if (type[1] == 'x') {
			tupletValue /= 6;
			note = convertDoubleToSymbol(tupletValue);
		}
		else if (type[1] == 'p') {
			tupletValue /= 7;
			note = convertDoubleToSymbol(tupletValue);
		}
	}

	// determine number of times singular note needs to be replicated
	tuplet.erase();
	if (type[0] == 'd' || (type[0] == 'q' && type[1] == 'a'))
		for (int i = 0; i < 3; i++)
			tuplet += note + " ";
	else if (type[0] == 't')
		for (int i = 0; i < 2; i++)
			tuplet += note + " ";
	else
		for (int i = 0; i < 4; i++)
			tuplet += note + " ";

	tuplet.pop_back();
}

double computeTotalBeats(int timeSignatureDenominator, std::string simplifiedNotes) {
	double totalBeats = 0;
	int i = 0;
	int j = 0;

	while (i < simplifiedNotes.length()) {
		if (simplifiedNotes[i] != ' ') {
			std::string note;
			for (j = i; simplifiedNotes[j] >= 48 && simplifiedNotes[j] <= 57; j++)
				note.push_back(simplifiedNotes[j]);

			totalBeats += 1 / stod(note) * timeSignatureDenominator;
			i = j;
		}
		else
			i++;
	}

	return totalBeats;
}