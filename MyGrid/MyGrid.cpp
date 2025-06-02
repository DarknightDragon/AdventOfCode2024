#include "MyGrid.h"
#include <string>
#include <vector>
#include <iostream>
#include <stdexcept>
#include <sstream>

// Constructors

MyGrid::MyGrid( int numCols, std::string input ) {
	if ( numCols < 1 ) {
		throw std::invalid_argument( "Received non-positive column value." );
	}
	if ( input == "" ) {
		throw std::invalid_argument( "Empty string. Grid can't initialize" );
	}
	if ( input.size() % numCols != 0 ) {
		throw std::invalid_argument( "Input size does not support new column value. Violates rectangle grid." );
	}

	this->numCols = numCols;
	grid = split( input );
	numRows = grid.size() / numCols;
}

MyGrid::MyGrid( int numCols, std::string input, std::string delim ) {
	if ( numCols < 1 ) {
		throw std::invalid_argument( "Received non-positive column value." );
	}

	std::vector<std::string> temp = split( input, delim );

	if ( temp.size() % numCols != 0 ) {
		throw std::invalid_argument( "Grid size does not support column size. Violates rectangle grid" );
	}

	this->numCols = numCols;
	grid = temp;
	numRows = grid.size() / numCols;
}


MyGrid::MyGrid( int numCols, std::vector<std::string> vecInput ) {
	if ( numCols < 1 ) {
		throw std::invalid_argument( "Received non-positive column value." );
	}

	if ( vecInput.size() % numCols != 0 ) {
		throw std::invalid_argument( "Grid size does not support column size. Violates rectangle grid" );
	}

	this->numCols = numCols;
	grid = vecInput;
	numRows = grid.size() / numCols;
}

// Public Functions

void MyGrid::setColumns( int newCols ) {
	if ( newCols < 1 ) {
		throw std::invalid_argument( "Received non-positive column value." );
	}
	else if ( grid.size() % newCols != 0 ) {
		throw std::invalid_argument( "Current grid size does not support new column value. Violates rectangle grid." );
	}

	numCols = newCols;
	numRows = grid.size() / numCols;
}

void MyGrid::addRow( std::string input ) {
	if ( numCols == 0 ) {
		numCols = input.size();
	}
	else if ( input.size() != numCols ) {
		throw std::length_error( "Input size exceeds current column size. Violates rectangle grid." );
	}

	for ( auto const& value : input ) {
		grid.push_back( std::string{ value } );
	}
	numRows++;
}

void MyGrid::addRow( std::string input, std::string delim ) {
	addRow( split( input, delim ) );
}

void MyGrid::addRow( std::vector<std::string> input ) {
	if ( numCols == 0 ) {
		numCols = input.size();
	}
	else if ( input.size() != numCols ) {
		throw std::length_error( "Input size exceeds current column size. Violates rectangle grid." );
	}

	grid.insert( grid.end(), input.begin(), input.end() );
	numRows++;
}
void MyGrid::deleteRow() {
	for ( int i = 0; i < numCols; i++ ) {
		grid.pop_back();
	}
	numRows--;
}

void MyGrid::clear() {
	grid.clear();
	numCols = 0;
	numRows = 0;
}

int MyGrid::countWords( const std::string& match, std::string delim ) const {
	if ( match.size() < 1 ) {
		throw std::invalid_argument( "Received empty match string" );
	}
	if ( grid.size() == 0 ) {
		std::cout << "Grid is empty" << std::endl;
		return 0;
	}

	std::vector<std::string> matchVec;
	
	if ( delim == "" ) {
		matchVec = split( match );
	}
	else {
		matchVec = split( match, delim );
	}

	return countWords( matchVec );
}

int MyGrid::countWords( const std::vector<std::string>& vecInput ) const {
	if ( vecInput.size() < 1 ) {
		throw std::invalid_argument( "Received empty match input" );
	}
	if ( grid.size() == 0 ) {
		std::cout << "Grid is empty" << std::endl;
		return 0;
	}

	int words = 0;

	for ( int i = 0; i < grid.size(); i++ ) {
		// if current character isn't the same as first character of match, skip
		if ( grid.at( i ) != vecInput.at( 0 ) ) {
			continue;
		}

		if ( checkUpperLeft( vecInput, i ) ) {
			words++;
		}
		if ( checkUpper( vecInput, i ) ) {
			words++;
		}
		if ( checkUpperRight( vecInput, i ) ) {
			words++;
		}
		if ( checkLeft( vecInput, i ) ) {
			words++;
		}
		if ( checkRight( vecInput, i ) ) {
			words++;
		}
		if ( checkLowerLeft( vecInput, i ) ) {
			words++;
		}
		if ( checkLower( vecInput, i ) ) {
			words++;
		}
		if ( checkLowerRight( vecInput, i ) ) {
			words++;
		}
	}

	return words;
}

int MyGrid::countWordsCross( const std::string& match, std::string delim ) const {
	if ( match.size() < 1 ) {
		throw std::invalid_argument( "Received empty match string" );
	}
	if ( grid.size() == 0 ) {
		std::cout << "Grid is empty" << std::endl;
		return 0;
	}

	std::vector<std::string> matchVec;

	if ( delim == "" ) {
		matchVec = split( match );
	}
	else {
		matchVec = split( match, delim );
	}

	return countWordsCross( matchVec );
}


int MyGrid::countWordsCross( const std::vector<std::string>& vecInput ) const {
	if ( vecInput.size() < 1 ) {
		throw std::invalid_argument( "Received empty match input" );
	}
	if ( grid.size() == 0 ) {
		std::cout << "Grid is empty" << std::endl;
		return 0;
	}

	int words = 0;

	for ( int i = 0; i < grid.size(); i++ ) {
		// if current character isn't the same as first character of match, skip
		if ( grid.at( i ) != vecInput.at( 0 ) ) {
			continue;
		}

		const int lengthA = vecInput.size() - 1;
		if ( checkLowerRight( vecInput, i ) && ( checkLowerLeft( vecInput, i + lengthA ) || checkUpperRight( vecInput, i + ( numCols * lengthA ) ) ) ) {
			words++;
		}

		if ( checkUpperLeft( vecInput, i ) && ( checkLowerLeft( vecInput, i - ( numCols * lengthA ) ) || checkUpperRight( vecInput, i - lengthA ) ) ) {
			words++;
		}
	}

	return words;
}

// Private Helper Functions

std::vector<std::string> MyGrid::split( const std::string& s ) const {
	std::vector<std::string> result;

	for ( auto const& value : s ) {
		result.push_back( std::string { value } );
	}

	return result;
}

std::vector<std::string> MyGrid::split( const std::string& s, std::string delimiter ) const {
	std::vector<std::string> result;
	size_t pos_start = 0, pos_end, delim_len = delimiter.length();
	std::string token;

	while ( ( pos_end = s.find( delimiter, pos_start ) ) != std::string::npos ) {
		token = s.substr( pos_start, pos_end - pos_start );
		pos_start = pos_end + delim_len;
		result.push_back( token );
	}

	result.push_back( s.substr( pos_start ) );
	return result;
}

std::vector<std::string> MyGrid::split( const std::string& str, char delim ) const {
	std::vector<std::string> result;
	std::stringstream ss( str );
	std::string item;

	while ( getline( ss, item, delim ) ) {
		result.push_back( item );
	}

	return result;
}

bool MyGrid::checkUpperLeft( const std::vector<std::string>& match, int index ) const {
	// if word is longer than remaining space before left edge or top edge respectively, then there can't be a match
	if ( ( match.size() > ( index % numCols ) + 1 ) || ( match.size() > ( index / numCols ) + 1 ) ) {
		return false;
	}

	bool matchFound = true;
	for ( int i = 0; i < match.size(); i++ ) {
		if ( grid.at( index - ( ( numCols + 1 ) * i ) ) != match.at( i ) ) {
			matchFound = false;
			break;
		}
	}

	return matchFound;
}

bool MyGrid::checkUpper( const std::vector<std::string>& match, int index ) const {
	// if word is longer than remaining space before top edge, then there can't be a match
	if ( match.size() > ( index / numCols + 1 ) ) {
		return false;
	}

	bool matchFound = true;
	for ( int i = 0; i < match.size(); i++ ) {
		if ( grid.at( index - ( numCols * i ) ) != match.at( i ) ) {
			matchFound = false;
			break;
		}
	}

	return matchFound;
}

bool MyGrid::checkUpperRight( const std::vector<std::string>& match, int index ) const {
	// if word is longer than remaining space before right edge or top edge respectively, then there can't be a match
	if ( ( match.size() > numCols - ( index % numCols ) ) || ( match.size() > ( index / numCols ) + 1 ) ) {
		return false;
	}

	bool matchFound = true;
	for ( int i = 0; i < match.size(); i++ ) {
		if ( grid.at( index - ( ( numCols - 1 ) * i ) ) != match.at( i ) ) {
			matchFound = false;
			break;
		}
	}

	return matchFound;
}

bool MyGrid::checkLeft( const std::vector<std::string>& match, int index ) const {
	// if word is longer than remaining space before left edge, then there can't be a match
	if ( match.size() > ( index % numCols ) + 1 ) {
		return false;
	}

	bool matchFound = true;
	for ( int i = 0; i < match.size(); i++ ) {
		if ( grid.at( index - i ) != match.at( i ) ) {
			matchFound = false;
			break;
		}
	}

	return matchFound;
}

bool MyGrid::checkRight( const std::vector<std::string>& match, int index ) const {
	// if word is longer than remaining space before right edge, then there can't be a match
	if ( match.size() > numCols - ( index % numCols ) ) {
		return false;
	}

	bool matchFound = true;
	for ( int i = 0; i < match.size(); i++ ) {
		if ( grid.at( index + i ) != match.at( i ) ) {
			matchFound = false;
			break;
		}
	}

	return matchFound;
}

bool MyGrid::checkLowerLeft( const std::vector<std::string>& match, int index ) const {
	// if word is longer than remaining space before left edge or bottom edge respectively, then there can't be a match
	if ( ( match.size() > ( index % numCols ) + 1 ) || ( match.size() > numRows - ( index / numCols ) ) ) {
		return false;
	}

	bool matchFound = true;
	for ( int i = 0; i < match.size(); i++ ) {
		if ( grid.at( index + ( ( numCols - 1 ) * i ) ) != match.at( i ) ) {
			matchFound = false;
			break;
		}
	}

	return matchFound;
}

bool MyGrid::checkLower( const std::vector<std::string>& match, int index ) const {
	// if word is longer than remaining space before bottom edge respectively, then there can't be a match
	if ( match.size() > numRows - ( index / numCols ) ) {
		return false;
	}

	bool matchFound = true;
	for ( int i = 0; i < match.size(); i++ ) {
		if ( grid.at( index + ( numCols * i ) ) != match.at( i ) ) {
			matchFound = false;
			break;
		}
	}

	return matchFound;
}

bool MyGrid::checkLowerRight( const std::vector<std::string>& match, int index ) const {
	// if word is longer than remaining space before right edge or bottom edge respectively, then there can't be a match
	if ( ( match.size() > numCols - ( index % numCols ) ) || ( match.size() > numRows - ( index / numCols ) ) ) {
		return false;
	}

	bool matchFound = true;
	for ( int i = 0; i < match.size(); i++ ) {
		if ( grid.at( index + ( ( numCols + 1 ) * i ) ) != match.at( i ) ) {
			matchFound = false;
			break;
		}
	}

	return matchFound;
}

// Public Friend Functions

std::ostream& operator<< ( std::ostream& os, const MyGrid& grid ) {
	os << "Grid: " << grid.numRows << "x" << grid.numCols << std::endl;
	int counter = 0;
	for ( auto const value : grid.grid ) {
		os << value;
		if ( counter++ % grid.numCols != grid.numCols - 1 ) {
			os << " ";
		}
		else {
			os << std::endl;
		}
	}

	return os;
}