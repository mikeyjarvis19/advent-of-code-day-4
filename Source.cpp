#include <vector>
#include <fstream>
#include <iostream>
#include <string>
#include <regex>
#include <sstream>
#include <map>
#include <cctype>

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

typedef std::map<std::string, std::string> PassportFields; // E.g. {{"byr", "1989"}, {"ecl", "blue"}}

PassportFields passport_string_to_map(std::string const& passportString) {
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

// Return true if maybe_year has 4 digits and is in range [lo, hi]
bool is_valid_year(const std::string& maybe_year, int lo, int hi)
{
	bool ok = maybe_year.size() == 4 && std::all_of(
		begin(maybe_year), end(maybe_year), [](char c) { return std::isdigit(c) != 0; });

	if (ok)
	{
		int year = std::stoi(maybe_year);
		ok = lo <= year && year <= hi;
	}
	return ok;
}

bool passport_is_valid(PassportFields const& passportMap) {
	std::string requiredKeys[] = { "byr", "iyr", "eyr", "hgt", "hcl", "ecl", "pid" };
	bool passportValid = true;
	for (const auto& requiredKey : requiredKeys)
	{
		auto it = passportMap.find(requiredKey);
		if (it != passportMap.end())
		{
			if (requiredKey == "byr" && !is_valid_year(it->second, 1920, 2002)) return false;
		}
		else
		{
			return false;
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
	std::vector<PassportFields> passportMaps;
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