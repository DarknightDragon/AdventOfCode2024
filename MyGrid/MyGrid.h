#pragma once
#ifndef MY_GRID_FILE_H
#define MY_GRID_FILE_H

#include <string>
#include <vector>
#include <iostream>

class MyGrid {
private:
	std::vector<std::string> grid;
	int numCols{};
	int numRows{};

	// countWords helper functions
	bool checkUpperLeft(const std::vector<std::string>& match, int index ) const;
	bool checkUpper(const std::vector<std::string>& match, int index ) const;
	bool checkUpperRight(const std::vector<std::string>& match, int index ) const;
	bool checkLeft(const std::vector<std::string>& match, int index ) const;
	bool checkRight(const std::vector<std::string>& match, int index ) const;
	bool checkLowerLeft(const std::vector<std::string>& match, int index ) const;
	bool checkLower(const std::vector<std::string>& match, int index ) const;
	bool checkLowerRight(const std::vector<std::string>& match, int index ) const;
	// tokenizes string by delimeter into vector 
	std::vector<std::string> split( const std::string& str ) const;
	std::vector<std::string> split( const std::string& str, std::string delim ) const;
	std::vector<std::string> split( const std::string& str, char delim ) const;

public:
	MyGrid() = default;
	MyGrid( int numCols, std::string input );
	MyGrid( int numCols, std::string input, std::string delim );
	MyGrid( int numCols, std::vector<std::string> vecInput );

	void setColumns( int newCols );
	int getColumns() const { return numCols; }
	int getRows() const { return numRows; }
	void addRow( std::string input );
	void addRow( std::string input, std::string delim );
	void addRow( std::vector<std::string> input );
	void deleteRow();
	void clear();
	// Q4
	int countWords( const std::string& match, std::string delim = "" ) const;
	int countWords( const std::vector<std::string>& vecInput ) const;
	// Q4P2
	int countWordsCross( const std::string& match, std::string delim = "" ) const;
	int countWordsCross( const std::vector<std::string>& vecInput ) const;

	friend std::ostream& operator<< ( std::ostream& os, const MyGrid& grid );
};

#endif // MY_GRID_FILE_H