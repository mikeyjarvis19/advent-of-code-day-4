#include <vector>
#include <fstream>
#include <iostream>
#include <string>
#include <regex>
#include <sstream>
#include <map>


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

std::map<std::string, std::string> passport_string_to_map(std::string const& passportString) {
	std::stringstream passportStringstream(passportString);
	std::string segment;
	std::vector<std::string> seglist;

	while (std::getline(passportStringstream, segment, ' '))
	{
		seglist.push_back(segment);
	}
	std::map<std::string, std::string> myMap;
	for (int i = 0; i < seglist.size(); i++) {
		std::stringstream seglistStringStream(seglist[i]);
		std::string seglistSegment;
		std::vector<std::string> seglistList;
		while (std::getline(seglistStringStream, segment, ':'))
		{
			seglistList.push_back(segment);
		}
		// Split seglist strings into key,value using ':' char.
		myMap[seglistList[0]] = seglistList[1];
	}
	return myMap;
}

bool passport_is_valid(std::map<std::string, std::string> passportMap) {
	std::string requiredKeys[] = { "byr", "iyr", "eyr", "hgt", "hcl", "ecl", "pid" };
	bool passportValid = true;
	for (const auto& requiredKey : requiredKeys) {
		if (passportMap.count(requiredKey) == 0) { 
			passportValid = false; 
			break; 
		}
	}
	return passportValid;
}

void part_1() {
	std::ifstream fileContents("input.txt");
	std::stringstream buffer;
	buffer << fileContents.rdbuf();
	std::string fileString = buffer.str();
	auto passportStrings = split_passports(fileString);
	std::vector<std::map<std::string, std::string>> passportMaps;
	int validPassportCount = 0;
	int totalPassports = 0;
	for (int i = 0; i < passportStrings.size(); i++) { 
		passportMaps.push_back(passport_string_to_map(passportStrings[i]));
	}
	for (int i = 0; i < passportMaps.size(); i++) {
		totalPassports++;
		auto currentMap = passportMaps[i];
		if (passport_is_valid(currentMap)) {
			validPassportCount++;
		}
	}
	std::cout << "Total passports: " << totalPassports << "\n";
	std::cout << "Valid passports: " << validPassportCount;
}

int main() {
	part_1();
	//part_2();
}