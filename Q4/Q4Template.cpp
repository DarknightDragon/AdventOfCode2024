#include <iostream>
#include <fstream>
#include <string>
#include "MyTGrid.h"

int countWords( MyTGrid<char> grid, std::string input );
int countWordsCross( MyTGrid<char> grid, std::string input );
bool checkUpperLeft( MyTGrid<char> grid, std::string input, int row, int col );
bool checkUpper( MyTGrid<char> grid, std::string input, int row, int col );
bool checkUpperRight( MyTGrid<char> grid, std::string input, int row, int col );
bool checkLeft( MyTGrid<char> grid, std::string input, int row, int col );
bool checkRight( MyTGrid<char> grid, std::string input, int row, int col );
bool checkLowerLeft( MyTGrid<char> grid, std::string input, int row, int col );
bool checkLower( MyTGrid<char> grid, std::string input, int row, int col );
bool checkLowerRight( MyTGrid<char> grid, std::string input, int row, int col );

int main() {
	std::ifstream file( "Data.txt" );
	MyTGrid<char> grid;

	if ( !file.is_open() ) {
		std::cerr << "File error" << std::endl;
		return 1;
	}

	for ( std::string line; std::getline( file, line ); ) {
		std::vector<char> temp;
		temp.reserve( line.length() );
		for ( char c : line ) {
			temp.push_back( c );
		}
		grid.addRow( temp );
	}

	file.close();

	std::cout << "\"XMAS\" appears " << countWords( grid, "XMAS" ) << " times." << std::endl;
	std::cout << "\"MAS\" appears " << countWordsCross( grid, "MAS" ) << " times in an X pattern." << std::endl;

	return 0;
}

int countWords( MyTGrid<char> grid, std::string input ) {
	if ( input.size() == 0 ) {
		throw std::invalid_argument( "Received empty match input" );
	}
	if ( grid.size() == 0 ) {
		std::cout << "Grid is empty" << std::endl;
		return 0;
	}

	int words = 0;

	for ( int i = 0; i < grid.getNumRows(); i++ ) {
		for ( int j = 0; j < grid.getNumColumns(); j++ ) {
			// if current character isn't the same as first character of match, skip
			if ( grid.at( i, j ) != input.at( 0 ) ) {
				continue;
			}

			if ( checkUpperLeft( grid, input, i, j ) ) {
				words++;
			}
			if ( checkUpper( grid, input, i, j ) ) {
				words++;
			}
			if ( checkUpperRight( grid, input, i, j ) ) {
				words++;
			}
			if ( checkLeft( grid, input, i, j ) ) {
				words++;
			}
			if ( checkRight( grid, input, i, j ) ) {
				words++;
			}
			if ( checkLowerLeft( grid, input, i, j ) ) {
				words++;
			}
			if ( checkLower( grid, input, i, j ) ) {
				words++;
			}
			if ( checkLowerRight( grid, input, i, j ) ) {
				words++;
			}
		}
	}

	return words;
}

int countWordsCross( MyTGrid<char> grid, std::string input ) {
	if ( input.size() < 1 ) {
		throw std::invalid_argument( "Received empty match input" );
	}
	if ( grid.size() == 0 ) {
		std::cout << "Grid is empty" << std::endl;
		return 0;
	}

	int words = 0;

	for ( int i = 0; i < grid.getNumRows(); i++ ) {
		for ( int j = 0; j < grid.getNumColumns(); j++ ) {
			// if current character isn't the same as first character of match, skip
			if ( grid.at( i, j ) != input.at( 0 ) ) {
				continue;
			}

			const int lengthA = input.size() - 1;
			if ( checkLowerRight( grid, input, i, j ) && ( checkLowerLeft( grid, input, i, j + lengthA ) || checkUpperRight( grid, input, i + lengthA, j ) ) ) {
				words++;
			}

			if ( checkUpperLeft( grid, input, i, j ) && ( checkLowerLeft( grid, input, i -  lengthA, j ) || checkUpperRight( grid, input, i, j - lengthA ) ) ) {
				words++;
			}
		}
	}

	return words;
}

bool checkUpperLeft( MyTGrid<char> grid, std::string input, int row, int col ) {
	// if word is longer than remaining space before left edge or top edge respectively, then there can't be a match
	if ( input.size() - 1 > col || input.size() - 1 > row ) {
		return false;
	}

	bool matchFound = true;
	for ( int i = 0; i < input.size(); i++ ) {
		if ( grid.at( row - i, col - i ) != input.at( i ) ) {
			matchFound = false;
			break;
		}
	}

	return matchFound;
}

bool checkUpper( MyTGrid<char> grid, std::string input, int row, int col ) {
	// if word is longer than remaining space before top edge, then there can't be a match
	if ( input.size() - 1 > row ) {
		return false;
	}

	bool matchFound = true;
	for ( int i = 0; i < input.size(); i++ ) {
		if ( grid.at( row - i, col ) != input.at( i ) ) {
			matchFound = false;
			break;
		}
	}

	return matchFound;
}

bool checkUpperRight( MyTGrid<char> grid, std::string input, int row, int col ) {
	// if word is longer than remaining space before right edge or top edge respectively, then there can't be a match
	if ( input.size() > grid.getNumColumns() - col || input.size() - 1 > row ) {
		return false;
	}

	bool matchFound = true;
	for ( int i = 0; i < input.size(); i++ ) {
		if ( grid.at( row - i, col + i ) != input.at( i ) ) {
			matchFound = false;
			break;
		}
	}

	return matchFound;
}

bool checkLeft( MyTGrid<char> grid, std::string input, int row, int col ) {
	// if word is longer than remaining space before left edge, then there can't be a match
	if ( input.size() - 1 > col ) {
		return false;
	}

	bool matchFound = true;
	for ( int i = 0; i < input.size(); i++ ) {
		if ( grid.at( row, col - i ) != input.at( i ) ) {
			matchFound = false;
			break;
		}
	}

	return matchFound;
}

bool checkRight( MyTGrid<char> grid, std::string input, int row, int col ) {
	// if word is longer than remaining space before right edge, then there can't be a match
	if ( input.size() > grid.getNumColumns() - col ) {
		return false;
	}

	bool matchFound = true;
	for ( int i = 0; i < input.size(); i++ ) {
		if ( grid.at( row, col + i ) != input.at( i ) ) {
			matchFound = false;
			break;
		}
	}

	return matchFound;
}

bool checkLowerLeft( MyTGrid<char> grid, std::string input, int row, int col ) {
	// if word is longer than remaining space before left edge or bottom edge respectively, then there can't be a match
	if ( input.size() - 1 > col || input.size() > grid.getNumRows() - row ) {
		return false;
	}

	bool matchFound = true;
	for ( int i = 0; i < input.size(); i++ ) {
		if ( grid.at( row + i, col - i ) != input.at( i ) ) {
			matchFound = false;
			break;
		}
	}

	return matchFound;
}

bool checkLower( MyTGrid<char> grid, std::string input, int row, int col ) {
	// if word is longer than remaining space before bottom edge respectively, then there can't be a match
	if ( input.size() > grid.getNumRows() - row ) {
		return false;
	}

	bool matchFound = true;
	for ( int i = 0; i < input.size(); i++ ) {
		if ( grid.at( row + i, col ) != input.at( i ) ) {
			matchFound = false;
			break;
		}
	}

	return matchFound;
}

bool checkLowerRight( MyTGrid<char> grid, std::string input, int row, int col ) {
	// if word is longer than remaining space before right edge or bottom edge respectively, then there can't be a match
	if ( input.size() > grid.getNumColumns() - col || input.size() > grid.getNumRows() - row ) {
		return false;
	}

	bool matchFound = true;
	for ( int i = 0; i < input.size(); i++ ) {
		if ( grid.at( row + i, col + i ) != input.at( i ) ) {
			matchFound = false;
			break;
		}
	}

	return matchFound;
}
