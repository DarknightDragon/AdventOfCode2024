#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <map> // multimap
#include <regex>

void printMMap( std::multimap<unsigned int, unsigned int> mmap );
void printMMap( std::multimap<unsigned int, unsigned int> mmap, int key );
bool pageUpdatesOrderCorrect( const std::multimap<unsigned int, unsigned int>& orderRules, const std::vector<unsigned int>& pageUpdates );
void fixPageUpdatesOrder( const std::multimap<unsigned int, unsigned int>& orderRules, std::vector<unsigned int>& pageUpdates );

int main() {
	std::ifstream file( "Data.txt" );
	// hashmap that allows many values per key; stored as a key/value pair making multiple entries of a single key for each value (not linked list)
	std::multimap<unsigned int, unsigned int> orderRules;
	std::regex patternOrderRules{ R"((\d+)\|(\d+))" };
	std::regex patternPageUpdates{ R"((\d+))" };
	std::vector<unsigned int> pageUpdates;
	const std::sregex_token_iterator endIterator;
	// Part 1
	int sumCorrectPageUpdates{ 0 };
	// Part 2
	int sumIncorrectPageUpdates{ 0 };

	if ( !file.is_open() ) {
		std::cerr << "File error" << std::endl;
		return 1;
	}

	bool inputBreak{ false };
	for ( std::string line; std::getline( file, line ); ) {
		bool pageUpdateCorrect{ true };
		if ( line == "" ) {
			inputBreak = true;
			continue;
		}
		// load page ordering rules
		if ( !inputBreak ) {
			std::sregex_token_iterator matchOrderRules{ line.begin(), line.end(), patternOrderRules, {1,2} };
			// regex guarantees two group matches per find
			orderRules.insert( { std::stoi( *matchOrderRules++ ), std::stoi( *matchOrderRules++ ) } );
		}
		else { // load page numbers of each update
			pageUpdates.reserve( line.length() );
			std::sregex_token_iterator matchPageUpdates{ line.begin(), line.end(), patternPageUpdates };

			while ( matchPageUpdates != endIterator ) {
				pageUpdates.push_back( std::stoi( *matchPageUpdates++ ) );
			}

			if ( pageUpdatesOrderCorrect( orderRules, pageUpdates ) ) {
				sumCorrectPageUpdates += pageUpdates.at( pageUpdates.size() / 2 );
			}
			else {
				fixPageUpdatesOrder( orderRules, pageUpdates );
				sumIncorrectPageUpdates += pageUpdates.at( pageUpdates.size() / 2 );
			}

			pageUpdates.clear();
		}
	}

	file.close();
	
	//printMMap( orderRules );
	std::cout << "The sum of the middle terms of the correct page updates is " << sumCorrectPageUpdates << "\n";
	std::cout << "The sum of the middle terms of the incorrect page updates is " << sumIncorrectPageUpdates << std::endl;

	return 0;
}

bool pageUpdatesOrderCorrect( const std::multimap<unsigned int, unsigned int>& orderRules, const std::vector<unsigned int>& pageUpdates )
{
	bool pageUpdateCorrect{ true };
	// skip first value since there is no value before it
	for ( int i = 1; i < pageUpdates.size(); i++ ) {
		for ( int j = 0; j < i; j++ ) {
			// auto [...] is called structured binding, only in C++ 17+
			// iterates through all values with given key
			for ( auto [itr, endItr] = orderRules.equal_range( pageUpdates.at( i ) ); itr != endItr; itr++ ) {
				if ( pageUpdates.at( j ) == itr->second ) {
					pageUpdateCorrect = false;
				}

				if ( !pageUpdateCorrect ) {
					break;
				}
			}
			if ( !pageUpdateCorrect ) {
				break;
			}
		}
		if ( !pageUpdateCorrect ) {
			break;
		}
	}

	return pageUpdateCorrect;
}

void fixPageUpdatesOrder( const std::multimap<unsigned int, unsigned int>& orderRules, std::vector<unsigned int>& pageUpdates )
{
	do {
		// skip first value since there is no value before it
		for ( int i = 1; i < pageUpdates.size(); i++ ) {
			for ( int j = 0; j < i; j++ ) {
				// auto [...] is called structured binding, only in C++ 17+
				// iterates through all values with given key
				for ( auto [itr, endItr] = orderRules.equal_range( pageUpdates.at( i ) ); itr != endItr; itr++ ) {
					if ( pageUpdates.at( j ) == itr->second ) {
						// swap the pages
						int temp = pageUpdates.at( j );
						pageUpdates.at( j ) = pageUpdates.at( i );
						pageUpdates.at( i ) = temp;
					}
				}
			}
		}
	} while ( !pageUpdatesOrderCorrect( orderRules, pageUpdates ) );
}

void printMMap( std::multimap<unsigned int, unsigned int> mmap ) {
	// incrementing by upper_bound instead of ++ allows looping through only unique keys instead of all keys
	// multiple values per key are input as 'rows' of key, value pairs; making multiple entries of a single key for each value
	for ( auto it = mmap.begin(), end = mmap.end(); it != end; it = mmap.upper_bound( it->first ) ) {
		std::cout << it->first << ": " << it->second;
		if ( mmap.count( it->first ) > 1 ) {
			bool isFirstEntry{ true };
			// auto [...] is called structured binding, only in C++ 17+
			// iterates through all values with given key
			for ( auto [itr, endItr] = mmap.equal_range( it->first ); itr != endItr; itr++ ) {
				// skip first value since it's been printed already
				if ( isFirstEntry ) {
					isFirstEntry = false;
				}
				std::cout << ", " << itr->second;
			}
		}
		std::cout << std::endl;
	}
}

void printMMap( std::multimap<unsigned int, unsigned int> mmap, int key ) {
	std::cout << key << ": " << mmap.find( key )->first;
	if ( mmap.count( key ) > 1 ) {
		bool isFirstEntry{ true };
		// auto [...] is called structured binding, only in C++ 17+
		// iterates through all values with given key
		for ( auto [itr, endItr] = mmap.equal_range( key ); itr != endItr; itr++ ) {
			if ( isFirstEntry ) {
				isFirstEntry = false;
			}
			std::cout << ", " << itr->second;
		}
	}
	std::cout << std::endl;
}
