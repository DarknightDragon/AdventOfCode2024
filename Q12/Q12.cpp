#include "MyTGrid.h"
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <utility> // std::pair
#include <algorithm> // std::sort

class Region {
private:
	unsigned int perimeter{ 0 };
	unsigned int area{ 0 };
	std::vector<std::pair<unsigned int, unsigned int>> coords;

	bool connectedAbove( std::pair<unsigned int, unsigned int> coord ) {
		bool ans = false;

		for ( int i = 0; i < coords.size(); i++ ) {
			if ( coords.at( i ).first < coord.first - 1 ) {
				continue;
			}
			else if ( coords.at( i ).first >= coord.first ) {
				break;
			}
			else if ( coords.at( i ).second > coord.second ) {
				break;
			}
			else if ( coords.at( i ).first == coord.first - 1 && coords.at( i ).second == coord.second ) {
				ans = true;
			}
		}

		return ans;
	}

	bool connectedAbove( unsigned int row, unsigned int col ) { return connectedAbove( { row, col } ); }

	bool connectedBelow( std::pair<unsigned int, unsigned int> coord ) {
		bool ans = false;

		for ( int i = 0; i < coords.size(); i++ ) {
			if ( coords.at( i ).first <= coord.first ) {
				continue;
			}
			else if ( coords.at( i ).first > coord.first + 1 ) {
				break;
			}
			else if ( coords.at( i ).second > coord.second ) {
				break;
			}
			else if ( coords.at( i ).first == coord.first + 1 && coords.at( i ).second == coord.second ) {
				ans = true;
			}
		}

		return ans;
	}

	bool connectedBelow( unsigned int row, unsigned int col ) { return connectedBelow( { row, col } ); }

	bool connectedLeft( std::pair<unsigned int, unsigned int> coord ) {
		bool ans = false;

		for ( int i = 0; i < coords.size(); i++ ) {
			if ( coords.at( i ).first < coord.first ) {
				continue;
			}
			else if ( coords.at( i ).first > coord.first ) {
				break;
			}
			else if ( coords.at( i ).second >= coord.second ) {
				break;
			}
			else if ( coords.at( i ).first == coord.first && coords.at( i ).second == coord.second - 1 ) {
				ans = true;
			}
		}

		return ans;
	}

	bool connectedLeft( unsigned int row, unsigned int col ) { return connectedLeft( { row,col } ); }

	bool connectedRight( std::pair<unsigned int, unsigned int> coord ) {
		bool ans = false;

		for ( int i = 0; i < coords.size(); i++ ) {
			if ( coords.at( i ).first < coord.first ) {
				continue;
			}
			else if ( coords.at( i ).first > coord.first ) {
				break;
			}
			else if ( coords.at( i ).second > coord.second + 1 ) {
				break;
			}
			else if ( coords.at( i ).first == coord.first && coords.at( i ).second == coord.second + 1 ) {
				ans = true;
			}
		}

		return ans;
	}

	bool connectedRight( unsigned int row, unsigned int col ) { return connectedRight( { row, col } ); }

public:
	Region() = default;

	Region( std::vector<std::pair<unsigned int, unsigned int>> inVec ) {
		std::sort( inVec.begin(), inVec.end() );
		coords.swap( inVec );
		area = coords.size();
	}

	unsigned int getPerimeter() { return perimeter; }

	unsigned int getArea() { return area; }

	unsigned int getPrice() { return area * perimeter; }

	// implementation only if debugging needed
	/*std::string toString() {
		// new row = old row * 2 + 1
		// new col = old col * 2 + 1
		std::string s;

		return s;
	}*/
};

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