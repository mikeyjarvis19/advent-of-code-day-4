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

bool is_valid_height(std::string const& height) {
	auto index_of_height_unit_start = height.find_first_of("ci");
	// This is a bit jank
	if (index_of_height_unit_start > 10) { return false; }
	auto unit_string = height.substr(index_of_height_unit_start, 2);
	auto lo = 0;
	auto hi = 0;
	if (unit_string == "cm") {
		lo = 150;
		hi = 193;
	}
	else if (unit_string == "in") {
		lo = 59;
		hi = 76;
	}
	auto height_num = std::stoi(height.substr(0, index_of_height_unit_start));
	auto valid = lo <= height_num && height_num <= hi;
	return valid;
}

bool is_valid_hair_colour(std::string hair_colour) {
	if (hair_colour[0] != '#') { return false; }
	if (hair_colour.size() != 7) { return false; }
	auto colour_characters = hair_colour.substr(1, 7);
	bool valid = false;
	std::vector<char> allowed_characters = { 'a', 'b', 'c', 'd', 'e', 'f', '0', '1', '2', '3', '4', '5', '6', '7', '8', '9' };
	//for (int i = 0; i < hair_colour.size(); i++) {
	//	for (int j = 0; j < allowed_characters.size(); j++) {
	//		if hair_colour
	//}
	//for (int i = 0; i < hair_colour.size(); i++) {
	//	auto current_char = allowed_characters[i];
	//	if (std::any_of(allowed_characters.begin(), allowed_characters.end(), [](int i) {return allowed_characters[i] == a"; }

	return true;
}

bool is_valid_eye_colour(std::string const& eye_colour) {
	std::string allowed_eye_colours [7]  = { "amb", "blu", "brn", "gry", "grn", "hzl", "oth" };
	for (const auto& colour : allowed_eye_colours)
	{
		if (colour == eye_colour) { return true; }
	}
	return false;
}

bool is_valid_passport_id(std::string const& passport_id) {
	if (passport_id.size() != 9) { return false; }
	for (const auto& character : passport_id) {
		if (std::isdigit(character) == 0) { return false; }
	}
	return true;
}

bool passport_is_valid(PassportFields const& passportMap) {
	std::string requiredKeys[] = { "ecl", "iyr", "eyr", "hgt", "hcl", "pid", "byr" };
	bool passportValid = true;
	
	// Are we missing the final iteration in this for?
	for (const auto& requiredKey : requiredKeys)
	{
		auto it = passportMap.find(requiredKey);
		if (it != passportMap.end())
		{
			if (requiredKey == "byr" && !is_valid_year(it->second, 1920, 2002)) return false;
			if (requiredKey == "iyr" && !is_valid_year(it->second, 2010, 2020)) return false;
			if (requiredKey == "eyr" && !is_valid_year(it->second, 2020, 2030)) return false;
			if (requiredKey == "hgt" && !is_valid_height(it->second)) return false;
			if (requiredKey == "hcl" && !is_valid_hair_colour(it->second)) return false;
			if (requiredKey == "ecl" && !is_valid_eye_colour(it->second)) return false;
			if (requiredKey == "pid" && !is_valid_passport_id(it->second)) return false;
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