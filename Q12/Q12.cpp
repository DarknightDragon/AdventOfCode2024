#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include "MyTGrid.h"

int main() {
	std::ifstream file( "TestData.txt" );
	MyTGrid<char> grid;

	if ( !file.is_open() ) {
		std::cerr << "File error" << std::endl;
		return 1;
	}

	for ( std::string line; std::getline( file, line ); ) {
		std::vector<char> row;
		row.reserve( line.length() );
		for ( char c : line ) {
			row.emplace_back( c );
		}
		grid.addRow( row );
	}

	file.close();


	return 0;
}