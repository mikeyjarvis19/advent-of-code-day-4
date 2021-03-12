#include <vector>
#include <fstream>
#include <iostream>
#include <string>
#include <regex>
#include <sstream>
//#include <algorithm>


std::vector<std::string> split_passports(std::string const& input_string)
{
	std::vector<std::string> passportStrings;
	std::regex re("[\r\n]{2,}");

	std::sregex_token_iterator
		begin(input_string.begin(), input_string.end(), re, -1),
		end;

	std::copy(begin, end, std::back_inserter(passportStrings));

	// Cleanup the newlines and replace with space character for consistency
	for (int i = 0; i < passportStrings.size(); i++) {
		for (size_t j = 0; j < passportStrings[i].length(); j++)
		{
			if (passportStrings[i][j] == '\n')
			{
				passportStrings[i][j] = ' ';
			}
		}
	}
	return passportStrings;
	}

void part_1() {
	std::ifstream fileContents("input.txt");
	std::stringstream buffer;
	buffer << fileContents.rdbuf();
	std::string fileString = buffer.str();
	auto passportStrings = split_passports(fileString);
	for (int i = 0; i < passportStrings.size(); i++) { std::cout << passportStrings[i] << "\n"; }
}

int main() {
	part_1();
	//part_2();
}