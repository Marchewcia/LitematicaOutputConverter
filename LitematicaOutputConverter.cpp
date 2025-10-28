#include <algorithm>
#include <format>
#include <fstream>
#include <iostream>
#include <map>
#include <string>
#include <vector>

const int ASCIInull = 48, ASCIInine = 57, rowsBeforeData = 5, amountInStack = 64, amountInShulk = 27, amountInVault = 54;
const char charAtEndOfData = '+';
const std::string empty = "";
const enum CurrentNum {
	First = 1,
	Second = 2,
	Third = 3
};

static void writeToFile(std::ofstream& file, std::string text, int num) {
	file << std::format("{}: {}", text, num);
}

int main() {
	bool readingNumber, endOfData = false;
	int i = 0;
	std::map<int, std::string> data;
	std::string input;

	std::cout << "Full path to file: ";
	std::getline(std::cin, input);
	std::fstream inpfile(input);

	std::cout << "Full path to output: ";
	std::getline(std::cin, input);
	std::ofstream output(input);

	while (std::getline(inpfile, input)) {
		if (i < rowsBeforeData) {
			output << input << "\n";
			i++;
			continue;
		}

		if (input[0] == charAtEndOfData && !endOfData) {
			endOfData = true;
			std::vector<int> keys;
			for (auto const& m : data) {
				keys.push_back(m.first);
			}
			std::sort(keys.begin(), keys.end(), std::greater<int>());
			for (int i : keys) {
				for (auto const& m : data) {
					if (m.first == i) {
						int full = i, stack = 0, shulk = 0, vault = 0;
						while (full >= amountInStack) {
							full -= amountInStack;
							stack++;
							if (stack == amountInShulk) {
								stack = 0;
								shulk++;
							}
							if (shulk == amountInVault) {
								vault++;
								shulk = 0;
							}
						}
						output << m.second;
						if (vault) {
							writeToFile(output, "Vaults", vault);
						}
						if (shulk) {
							vault ? writeToFile(output, ", Shulkers", shulk) : writeToFile(output, "Shulkers", shulk);
						}
						if (stack) {
							vault || shulk ? writeToFile(output, ", Stacks", stack) : writeToFile(output, "Stacks", stack);
						}
						if (full) {
							vault || shulk || stack ? writeToFile(output, ", Rest", full) : writeToFile(output, "Rest", full);
						}
						output << "\n";
					}
				}
			}
		}

		if (endOfData) {
			output << input << std::endl;
			continue;
		}

		int unplaced = 0, available = 0;
		std::string readNumber = empty;
		enum CurrentNum currentlyReadedNumber = First;

		for (char c : input) {
			readingNumber = false;
			if (int(c) >= ASCIInull && int(c) <= ASCIInine) {
				readNumber += c;
				readingNumber = true;
			}
			if (!readingNumber && readNumber != empty) {
				switch (currentlyReadedNumber) {
				case First:
					currentlyReadedNumber = Second;
					break;
				case Second:
					unplaced = std::stoi(readNumber);
					currentlyReadedNumber = Third;
					break;
				case Third:
					available = std::stoi(readNumber);
					data.insert({ unplaced - available, input });
					break;
				}
				readNumber = empty;
			}
		}
	}
}