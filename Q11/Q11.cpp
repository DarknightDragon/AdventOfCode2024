#include <iostream>
#include <fstream>
#include <string>
#include <list> // std::list
#include <regex>
#include <cmath> // std::logBASE()

unsigned long long getNumDigits( unsigned long long n );
void blink( std::list<unsigned long long>& list, unsigned n = 1 );

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

	blink( list, 75 );
	std::cout << "There are " << list.size() << " stones" << std::endl;
	//for ( auto num : list ) { std::cout << num << " "; }

	return 0;
}

unsigned long long getNumDigits( unsigned long long n ) {
	return static_cast<unsigned long long>( std::log10l( n ) ) + 1;
}

void blink( std::list<unsigned long long>& list, unsigned n ) {
	if ( list.size() == 0 ) {
		return;
	}

	int i = 0;
	for ( int i = 0; i < n; i++ ) {
		for ( auto listIt = list.begin(); listIt != list.end(); listIt++ ) {
			unsigned long long numDigits = getNumDigits( *listIt );
			if ( *listIt == 0 ) {
				*listIt = 1ull;
			}
			else if ( numDigits % 2 == 0 ) {
				unsigned int placeHolder = static_cast<unsigned int>( std::pow( 10, ( numDigits / 2 ) ) );
				list.insert( listIt, static_cast<unsigned long long>( *listIt / placeHolder ) );
				*listIt = *listIt % placeHolder;
			}
			else {
				*listIt *= 2024;
			}
		}
	}
}