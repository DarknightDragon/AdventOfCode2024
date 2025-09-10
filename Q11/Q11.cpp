#include <iostream>
#include <fstream>
#include <string>
#include <list> // std::list
#include <regex>
#include <cmath> // std::logBASE()
#include <unordered_map> // std::unordered_map

unsigned long long getNumDigits( unsigned long long n );
unsigned long long blink( std::list<unsigned long long> list, unsigned n = 1 );
unsigned long long blink_part2( const std::list<unsigned long long>& list, unsigned n = 1 );

int main() {
	std::ifstream file( "Data.txt" );
	std::list<unsigned long long> list;
	std::string line;

	if ( !file.is_open() ) {
		std::cerr << "File error" << std::endl;
		return 1;
	}

	// file is a single line
	std::getline( file, line );
	file.close();

	std::regex pattern{ R"((\d+))" };
	std::sregex_token_iterator it{ line.begin(), line.end(), pattern };
	std::sregex_token_iterator endIt;

	while ( it != endIt ) {
		list.emplace_back( std::stoull( *it++ ) );
	}

	std::cout << "Part 1: There are " << blink( list, 25 ) << " stones" << std::endl;
	std::cout << "Part 2: There are " << blink_part2( list, 75 ) << " stones" << std::endl;

	return 0;
}

unsigned long long getNumDigits( unsigned long long n ) {
	return static_cast<unsigned long long>( std::log10l( n ) ) + 1;
}

unsigned long long blink( std::list<unsigned long long> list, unsigned n ) {
	if ( list.size() == 0 ) {
		return 0;
	}

	int i = 0;
	for ( int i = 0; i < n; i++ ) {
		for ( auto listIt = list.begin(); listIt != list.end(); listIt++ ) {
			unsigned long long numDigits = getNumDigits( *listIt );
			if ( *listIt == 0 ) {
				*listIt = 1ull;
			}
			else if ( numDigits % 2 == 0 ) {
				unsigned int placeholder = static_cast<unsigned int>( std::pow( 10, ( numDigits / 2 ) ) );
				list.insert( listIt, static_cast<unsigned long long>( *listIt / placeholder ) );
				*listIt = *listIt % placeholder;
			}
			else {
				*listIt *= 2024;
			}
		}
	}

	return list.size();
}

unsigned long long blink_part2( const std::list<unsigned long long>& list, unsigned n ) {
	unsigned long long total{ 0ull };

	std::unordered_map<unsigned long long, unsigned long long> currMap;
	std::unordered_map<unsigned long long, unsigned long long> nextMap;

	// fill list into map
	for ( const auto& num : list ) {
		if ( currMap.find( num ) == currMap.end() ) {
			currMap.insert( { num, 1ull } );
		}
		else {
			++currMap.at( num );
		}
	}

	for ( int i = 0; i < n; i++ ) {
		for ( auto num : currMap ) {
			// rule 1: if 0, then becomes 1
			if ( num.first == 0 ) {
				if ( nextMap.find( 1ull ) == nextMap.end() ) {
					nextMap.insert( { 1ull, num.second } );
				}
				else {
					nextMap.at( 1 ) += num.second;
				}
			}
			// rule 2: if even digits, split into two numbers
			// C++ 17+ allows initialization in if
			else if ( auto numDigits = getNumDigits( num.first ); numDigits % 2 == 0 ) {
				unsigned long long placeholder = static_cast<unsigned long long>( std::pow( 10, numDigits / 2 ) );
				unsigned long long leftNum = num.first / placeholder;
				if ( nextMap.find( leftNum ) == nextMap.end() ) {
					nextMap.insert( { leftNum, num.second } );
				}
				else {
					nextMap.at( leftNum ) += num.second;
				}
				unsigned long long rightNum = num.first % placeholder;
				if ( nextMap.find( rightNum ) == nextMap.end() ) {
					nextMap.insert( { rightNum, num.second } );
				}
				else {
					nextMap.at( rightNum ) += num.second;
				}

			}
			// rule 3: multiply num by 2024
			else {
				unsigned long long newNum = num.first * 2024;
				if ( nextMap.find( newNum ) == nextMap.end() ) {
					nextMap.insert( { newNum, num.second } );
				}
				else {
					nextMap.at( newNum ) += num.second;
				}
			}
		} // close for each num in currMap

		// next map has the next current list, swap them and clear nextMap for next list
		currMap.swap( nextMap );
		nextMap.clear();
	}

	for ( auto num : currMap ) {
		total += num.second;
	}
	return total;
}