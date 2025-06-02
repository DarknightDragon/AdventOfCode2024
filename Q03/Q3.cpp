#include <iostream>
#include <fstream>
#include <string>
#include <regex>

int main() {
	std::ifstream file( "Data.txt" );
	bool doOn = true;

	if ( !file.is_open() ) {
		std::cerr << "File error" << std::endl;
	}

	long long totalA = 0LL;
	long long totalB = 0LL;	

	for ( std::string line; std::getline( file, line ); ) {
		// pattern made with string literal
		// part 1
		std::regex patternA{ R"(mul\((\d{1,3}),(\d{1,3})\))" };
		std::sregex_token_iterator numIteratorA{ line.begin(), line.end(), patternA, {1,2} };
		// part 2
		std::regex patternB{ R"((mul\(\d{1,3},\d{1,3}\))|(don't\(\))|(do\(\)))" };
		std::sregex_token_iterator numIteratorB{ line.begin(), line.end(), patternB };
		// needed to track when at the end of numIterator
		std::sregex_token_iterator endIterator;

		while ( numIteratorA != endIterator ) {
			// the regex guarantees two group matches per find
			totalA += std::stoll( *numIteratorA++ ) * std::stoll( *numIteratorA++ );
		}

		while ( numIteratorB != endIterator ) {
			std::string match = *numIteratorB;
			if ( doOn ) {
				// don't, turn it off
				if ( match.length() == 7 ) {
					doOn = false;
				}
				else if ( match.length() > 7 ) {
					short num1 = -1, num2 = -1;
					short del = match.find( "," );
					num1 = std::stoi( match.substr(4, del - 4 ));
					num2 = std::stoi( match.substr( del + 1 , match.find( ")" ) - del + 1 ) );
					totalB += num1 * num2;
				}
			}
			else if ( match.length() == 4 ) {
				// do, turn it on
					doOn = true;
			}

			++numIteratorB;
		}

	}
	file.close();

	std::cout << "Part 1 total is: " << totalA << std::endl;
	std::cout << "Part 2 total is: " << totalB << std::endl;
	return 0;
}