#include <iostream>
#include <fstream>
#include <string>
#include "MyGrid.h"

int main1() {
	std::ifstream file( "Data.txt" );
	MyGrid grid;

	if ( !file.is_open() ) {
		std::cerr << "File error" << std::endl;
		return 1;
	}

	for ( std::string line; std::getline( file, line ); ) {
		grid.addRow( line );
	}

	file.close();

	std::cout << "\"XMAS\" appears " << grid.countWords( "XMAS" ) << " times." << std::endl;
	std::cout << "\"MAS\" appears " << grid.countWordsCross( "MAS" ) << " times in an X pattern." << std::endl;

	return 0;
}