// input consists of reports (lines) and levels (numbers) separated by spaces
#include <iostream>
#include <string>
#include <fstream>
#include <cmath>
#include <vector>
#include <sstream>

int main() {
	std::ifstream file( "Data.txt" );
	int numSafeReports = 0;
	std::vector<int> reports;
	reports.reserve( 5 );
	std::stringstream ss( "" );

	if ( file.is_open() ) {
		for ( std::string line; std::getline( file, line ); ) {
			ss.str( line );
			std::string nums;
			int numProblems = 0;
			int num1, num2 = -1;
			// increasing is 1, decreasing is -1, and uninitialized is 0
			int incOrDec = 0;
			bool pass = true;

			while ( ss >> nums ) {
				reports.push_back( std::stoi( nums ) );
			}

			num1 = reports[0];

			// pass 1, error is not first 2 nums or is second number without direction change
			for ( int i = 1; i < reports.size(); i++ ) {
				bool isError = false;
				num2 = reports[i];
				if ( num1 == num2 || std::abs( num1 - num2 ) > 3 || ( incOrDec == 1 && num1 > num2 ) || ( incOrDec == -1 && num1 < num2 ) ) {
					numProblems++;
					isError = true;
				}
				else if ( incOrDec == 0 ) {
					incOrDec = num1 < num2 ? 1 : -1;
				}
				if ( numProblems > 1 ) {
					pass = false;
					break;
				}
				if ( !isError ) {
					num1 = num2;
				}
			}

			// check if first number is error
			if ( !pass ) {
				pass = true;
				num1 = reports[1];
				incOrDec = 0;
				for ( int i = 2; i < reports.size(); i++ ) {
					num2 = reports[i];
					if ( num1 == num2 || std::abs( num1 - num2 ) > 3 ) {
						pass = false;
					}
					else if ( ( incOrDec == 1 && num1 > num2 ) || ( incOrDec == -1 && num1 < num2 ) ) {
						pass = false;
					}
					else if ( incOrDec == 0 ) {
						incOrDec = num1 < num2 ? 1 : -1;
					}

					if ( !pass ) {
						break;
					}
					num1 = num2;
				}
			}
			
			// check if second number is error
			if ( !pass ) {
				pass = true;
				num1 = reports[0];
				incOrDec = 0;
				for ( int i = 2; i < reports.size(); i++ ) {
					num2 = reports[i];
					if ( num1 == num2 || std::abs( num1 - num2 ) > 3 ) {
						pass = false;
					}
					else if ( ( incOrDec == 1 && num1 > num2 ) || ( incOrDec == -1 && num1 < num2 ) ) {
						pass = false;
					}
					else if ( incOrDec == 0 ) {
						incOrDec = num1 < num2 ? 1 : -1;
					}

					if ( !pass ) {
						break;
					}
					num1 = num2;
				}
			}

			if ( pass ) {
				numSafeReports++;
			}

			ss.clear();
			reports.clear();
			reports.reserve( 5 );

			/*int num1 = -1, num2 = -1;
			std::string temp = "";
			// increasing is 1, decreasing is -1, and uninitialized is 0
			int incOrDec = 0;
			// Part two
			int numProblems = 0;
			int firstNum = -1, secondNum = -1;

			// pull the integers out using delimeter space (' ')
			for ( int i = 0; i < line.length(); i++ ) {
				// need to handle if 1st or 2nd numbers are errors
				if ( numProblems > 1 ) {
					break;
				}
				if ( i == line.length() - 1 && line[i] != ' ' ) {
					temp += line[i];
				}
				if ( line[i] == ' ' || i == line.length() - 1 ) {
					// if num1 isn't assigned, assign it first
					if ( num1 == -1 ) {
						num1 = std::stoi( temp );
						firstNum = num1;
					}
					else {
						num2 = std::stoi( temp );
						if ( secondNum == -1 ) {
							secondNum = num2;
						}
						if ( num1 == num2 || std::abs( num1 - num2 ) > 3 ) {
							numProblems++;
							temp.clear();
							continue;
						}
						else if ( ( incOrDec == 1 && num1 > num2 ) || ( incOrDec == -1 && num1 < num2 ) ) {
							numProblems++;
							temp.clear();
							continue;
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

			if ( numProblems <= 1 ) {
				numSafeReports++;
			}*/
		}

		file.close();
	}

	std::cout << "Number of safe reports: " << numSafeReports << std::endl;

	return 0;
}