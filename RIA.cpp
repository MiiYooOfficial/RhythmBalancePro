#include <string>
#include <algorithm>

#include "RIA.h"
#include "Globals.h"
#include "Grouping.h"
#include "GeneralFunctions.h"

std::string RIA(const int timeSignatureNumerator, const int timeSignatureDenominator, Grouping beginningNotes, Grouping endingNotes) {
	bool* beatCompleted = new bool[timeSignatureNumerator];
	int totalBeats = 0;
	std::string beginningRests, middleRests, endingRests;

	for (int i = 0; i < timeSignatureNumerator; i++)
		beatCompleted[i] = false;

	// complete beats at the start of measure
	if (beginningNotes.getTotalValue() != 0) {
		double beginningBeats;
		if (modf(beginningNotes.getTotalValue(), &beginningBeats) != 0) {
			beginningRests = completeBeat(timeSignatureDenominator, beginningNotes);
			beginningBeats++;
		}

		if (beginningBeats > timeSignatureNumerator)
			return "measure overflow!";
		else
			for (int beat = 0; beat < beginningBeats; beat++)
				beatCompleted[beat] = true;

		totalBeats += int(beginningBeats);
	}
	if (beginningRests == ERROR)
		return "unable to solve problem";

	// complete beats at the end of measure
	if (endingNotes.getTotalValue() != 0) {
		double endingBeats;
		if (modf(endingNotes.getTotalValue(), &endingBeats) != 0) {
			endingRests = completeBeat(timeSignatureDenominator, endingNotes);
			reverseRests(endingRests);
			endingBeats++;
		}

		if (totalBeats + endingBeats > timeSignatureNumerator)
			return "measure overflow!";
		else
			for (int counter = 0, beat = timeSignatureNumerator - 1; counter < endingBeats; counter++, beat--)
				beatCompleted[beat] = true;

		totalBeats += int(endingBeats);
	}
	if (endingRests == ERROR)
		return "unable to solve problem";

	// fill in the remaining beats
	if (totalBeats == 0) {
		if (timeSignatureNumerator / timeSignatureDenominator >= 2)
			middleRests.append("11");
		else
			middleRests.append("1");
	}
	else
		middleRests = completeMeasure(timeSignatureNumerator, timeSignatureDenominator, totalBeats, beatCompleted);

	return beginningRests + middleRests + endingRests;
}

std::string completeBeat(const int timeSignatureDenominator, Grouping notes) {
	std::string rests;
	double completedBeats; // not important for beat completion
	double beatSoFar = modf(notes.getTotalValue(), &completedBeats);

	while (beatSoFar < 1) {
		if (beatSoFar <= 0.5) {
			if (validateDoubleAsRest(beatSoFar)) {
				rests.append(createRest(beatSoFar, timeSignatureDenominator) + " ");
				beatSoFar += beatSoFar;
			}
			else if (validateDoubleAsRest(0.5 - beatSoFar)) {
				rests.append(createRest(0.5 - beatSoFar, timeSignatureDenominator) + " ");
				beatSoFar += 0.5 - beatSoFar;
			}
			else { // continuously append a rest that matches the smallest-valued note from the notes grouping until another condition can be met
				int noteWithShortestValue = findNoteWithSmallestValue(notes);

				rests.append(createRest(timeSignatureDenominator / double(noteWithShortestValue), timeSignatureDenominator) + " ");
				beatSoFar += timeSignatureDenominator / double(noteWithShortestValue);
			}
		}
		else {
			if (validateDoubleAsRest(1 - beatSoFar)) {
				rests.append(createRest(1 - beatSoFar, timeSignatureDenominator) + " ");
				beatSoFar += 1 - beatSoFar;
			}
			else if (validateDoubleAsRest(beatSoFar - 0.5)) {
				rests.append(createRest(beatSoFar - 0.5, timeSignatureDenominator) + " ");
				beatSoFar += beatSoFar - 0.5;
			}
			else { // continuously append a rest that matches the smallest-valued note from the notes grouping until another condition can be met
				int noteWithShortestValue = findNoteWithSmallestValue(notes);

				rests.append(createRest(timeSignatureDenominator / double(noteWithShortestValue), timeSignatureDenominator) + " ");
				beatSoFar += timeSignatureDenominator / double(noteWithShortestValue);
			}
		}
	}

	if (beatSoFar != 1)
		return ERROR;
	return rests;
}

bool validateDoubleAsRest(double restValue) {
	while (restValue < 1)
		restValue *= 2;

	if (restValue == 1)
		return true;
	return false;
}

std::string createRest(double absoluteValue, const int timeSignatureDenominator) {
	int relativeValue = 1;
	for (relativeValue = 1; relativeValue != timeSignatureDenominator; relativeValue *= 2)
		absoluteValue /= 2;

	return convertDoubleToSymbol(absoluteValue);
}

void reverseRests(std::string &rests) {
	std::string restsInOrder;

	int index = int(rests.length()) - 1;
	std::string singularRest;
	while (index >= 0) {
		if (rests[index] == ' ') {
			if (singularRest.length() != 0) {
				std::reverse(singularRest.begin(), singularRest.end());
				restsInOrder.append(singularRest + " ");
				singularRest.clear();
			}
			index--;
		}
		else {
			singularRest.push_back(rests[index]);
			index--;
		}
	}
	if (singularRest.length() > 1)
		std::reverse(singularRest.begin(), singularRest.end());
	restsInOrder.append(singularRest);

	rests.clear();
	rests.append(restsInOrder);
}

int findNoteWithSmallestValue(Grouping notes) {
	int noteWithShortestValue = 0;

	for (int charIndex = 0; charIndex < (notes.getSimplifiedNotes()).length(); charIndex++) {
		std::string note;
		while (notes.getSimplifiedNotes()[charIndex] != ' ' && charIndex < (notes.getSimplifiedNotes()).length()) {
			note.push_back(notes.getSimplifiedNotes()[charIndex]);
			charIndex++;
		}

		if (stoi(note) > noteWithShortestValue)
			noteWithShortestValue = stoi(note);
	}

	return noteWithShortestValue;
}

std::string completeMeasure(int timeSignatureNumerator, int timeSignatureDenominator, int totalBeats, bool* beatCompleted) {
	std::string answer;
	int numTriplePulseGroupings = 0;
	int numDoublePulseGroupings = 0;

	// preliminary measure processing for hybrid meters
	if (timeSignatureNumerator == 5 || timeSignatureNumerator == 7 || timeSignatureNumerator == 10 || timeSignatureNumerator == 11) {
		int pulse = 0;
		while (beatCompleted[pulse] != false)
			pulse++;
		if (pulse >= 2) {
			if (pulse % 3 == 0)
				numTriplePulseGroupings += pulse / 3;
			else if (pulse % 2 == 0)
				numDoublePulseGroupings += pulse / 2;
			else {
				while (pulse % 2 != 0) {
					numTriplePulseGroupings++;
					pulse -= 3;
				}
				numDoublePulseGroupings += pulse / 2;
			}
		}

		pulse = timeSignatureNumerator - 1;
		while (beatCompleted[pulse] != false)
			pulse--;
		if (timeSignatureNumerator - 1 - pulse >= 2) {
			if (timeSignatureNumerator - 1 - pulse % 3 == 0)
				numTriplePulseGroupings += pulse / 3;
			else if (timeSignatureNumerator - 1 - pulse % 2 == 0)
				numDoublePulseGroupings += pulse / 2;
			else {
				while ((timeSignatureNumerator - 1 - pulse) % 2 != 0) {
					numTriplePulseGroupings++;
					pulse += 3;
				}
				numDoublePulseGroupings += (timeSignatureNumerator - 1 - pulse) / 2;
			}
		}
	}

	// left-to-right execution for measure completion
	for (int i = 0; i < timeSignatureNumerator; i++) {
		if (timeSignatureNumerator - totalBeats == 1) { // no beat combinations possible
			answer.append(createRest(1, timeSignatureDenominator) + " ");
			totalBeats++;
		}
		else if (beatCompleted[i] == false) {
			if (timeSignatureNumerator == 3 || timeSignatureNumerator == 4) { // simple time
				if (i == 0 || (timeSignatureNumerator == 4 && i == 2)) {
					answer.append(createRest(2, timeSignatureDenominator) + " ");
					totalBeats += 2;
					i++;
				}
				else {
					answer.append(createRest(1, timeSignatureDenominator) + " ");
					totalBeats++;
				}
			}
			else if (timeSignatureNumerator == 6 || timeSignatureNumerator == 9 || timeSignatureNumerator == 12) { // compound time
				if (timeSignatureNumerator == 12 && i % 6 == 0) { // special possibility for compound quadruple time
					bool combineHalfMeasure = true;
					for (int pulse = i; pulse < i + 6; pulse++) {
						if (beatCompleted[pulse] == true) {
							combineHalfMeasure = false;
							break;
						}
					}
					if (combineHalfMeasure) {
						answer.append(createRest(4, timeSignatureDenominator) + "* ");
						totalBeats += 6;
						i += 5;
						continue;
					}
				}

				if (i % 3 == 0) {
					answer.append(createRest(2, timeSignatureDenominator));
					totalBeats += 2;

					if (beatCompleted[i + 2] == false) {
						answer.append("*");
						totalBeats++;
						i++;
					}

					i++;
					answer.append(" ");
				}
				else {
					answer.append(createRest(1, timeSignatureDenominator) + " ");
					totalBeats++;
				}
			}
			else if (timeSignatureNumerator == 5 || timeSignatureNumerator == 7 || timeSignatureNumerator == 10 || timeSignatureNumerator == 11) { // hybrid time
				if (i == 1) {
					answer.append(createRest(1, timeSignatureDenominator) + " ");
					totalBeats++;

					if (((timeSignatureNumerator == 5 || timeSignatureNumerator == 7) && numTriplePulseGroupings == 0) || (timeSignatureNumerator == 10 && numTriplePulseGroupings < 2) || (timeSignatureNumerator == 11 && numTriplePulseGroupings < 3)) {
						answer.append(createRest(1, timeSignatureDenominator) + " ");
						totalBeats++;
						numTriplePulseGroupings++;
						i++;
					}
					else
						numDoublePulseGroupings++;
				}
				else if (i == 2 && (((timeSignatureNumerator == 5 || timeSignatureNumerator == 7) && numTriplePulseGroupings == 0) || (timeSignatureNumerator == 10 && numTriplePulseGroupings < 2) || (timeSignatureNumerator == 11 && numTriplePulseGroupings < 3))) {
					answer.append(createRest(1, timeSignatureDenominator) + " ");
					totalBeats++;
					numTriplePulseGroupings++;

					if (numDoublePulseGroupings > 0)
						numDoublePulseGroupings--;
				}
				else {
					if (((timeSignatureNumerator == 5 || timeSignatureNumerator == 7) && numTriplePulseGroupings == 0) || (timeSignatureNumerator == 10 && numTriplePulseGroupings < 2) || (timeSignatureNumerator == 11 && numTriplePulseGroupings < 3)) {
						answer.append(createRest(2, timeSignatureDenominator));
						totalBeats += 2;

						if (beatCompleted[i + 2] == false) { // ensure that third beat of triple grouping is not already completed by trailing notes at end of bar
							answer.append("*");
							totalBeats++;
							i++;
						}

						numTriplePulseGroupings++;
						i++;
						answer.append(" ");
					}
					else if (((timeSignatureNumerator == 5 || timeSignatureNumerator == 11) && numDoublePulseGroupings == 0) || ((timeSignatureNumerator == 7 || timeSignatureNumerator == 10) && numDoublePulseGroupings < 2)) {
						answer.append(createRest(2, timeSignatureDenominator) + " ");
						totalBeats += 2;
						numDoublePulseGroupings++;
						i++;
					}
				}
			}
		}

		if (totalBeats == timeSignatureNumerator)
			break;
	}

	return answer;
}