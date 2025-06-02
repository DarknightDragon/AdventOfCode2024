// input consists of reports (lines) and levels (numbers) separated by spaces
#include <iostream>
#include <string>
#include <fstream>
#include <cmath> // std::abs

int main1() {
	std::ifstream file( "Data.txt" );
	int numSafeReports = 0;

	if ( file.is_open() ) {
		for ( std::string line; std::getline( file, line ); ) {
			int num1 = -1, num2 = -1;
			std::string temp = "";
			// increasing is 1, decreasing is -1, and uninitialized is 0
			int incOrDec = 0;
			bool reportIsSafe = true;

			// pull the integers out using delimeter space (' ')
			for ( int i = 0; i < line.length(); i++ ) {
				if ( i == line.length() - 1 && line[i] != ' ' ) {
					temp += line[i];
				}
				if ( line[i] == ' ' || i == line.length() - 1 ) {
					// if num1 isn't assigned, assign it first
					if ( num1 == -1 ) {
						num1 = std::stoi( temp );
					}
					else {
						num2 = std::stoi( temp );
						if ( num1 == num2 || std::abs( num1 - num2 ) > 3 ) {
							reportIsSafe = false;
							break;
						}
						else if ( ( incOrDec == 1 && num1 > num2 ) || ( incOrDec == -1 && num1 < num2 ) ) {
							reportIsSafe = false;
							break;
						}
						else if ( incOrDec == 0 ) {
							incOrDec = num1 < num2 ? 1 : -1;
						}
						num1 = num2;
					}
					// clear temp string to start next number
					temp.clear();
				}
				else {
					temp += line[i];
				}
			}

			if ( reportIsSafe ) {
				numSafeReports++;
			}
		}

		file.close();
	}

	std::cout << "Number of safe reports: " << numSafeReports << std::endl;

	return 0;
}