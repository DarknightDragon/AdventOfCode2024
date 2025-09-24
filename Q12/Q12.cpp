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
		if ( coord.first == 0 ) {
			return false;
		}

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
				break;
			}
		}

		return ans;
	}

	bool connectedAbove( unsigned int row, unsigned int col ) { return connectedAbove( { row, col } ); }

	bool connectedBelow( std::pair<unsigned int, unsigned int> coord ) {
		if ( coord.first >= coords.at( coords.size() - 1 ).first ) {
			return false;
		}

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
				break;
			}
		}

		return ans;
	}

	bool connectedBelow( unsigned int row, unsigned int col ) { return connectedBelow( { row, col } ); }

	bool connectedLeft( std::pair<unsigned int, unsigned int> coord ) {
		if ( coord.second == 0 ) {
			return false;
		}

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
				break;
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
				break;
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
		for ( int i = 0; i < coords.size(); i++ ) {
			perimeter += 4;
			if ( connectedAbove( coords.at( i ) ) ) { --perimeter; }
			if ( connectedBelow( coords.at( i ) ) ) { --perimeter; }
			if ( connectedLeft( coords.at( i ) ) ) { --perimeter; }
			if ( connectedRight( coords.at( i ) ) ) { --perimeter; }
		}
	}

	unsigned int getPerimeter() { return perimeter; }

	unsigned int getArea() { return area; }

	unsigned int getPrice() { return area * perimeter; }

	void add( std::pair<unsigned int, unsigned int> coord ) {
		short neighbors = 0;
		bool alreadyExists = false;
		for ( int i = 0; i < coords.size(); i++ ) {
			if ( coords.at( i ) == coord ) {
				alreadyExists = true;
				break;
			}
			// check if new coord is below existing coord
			if ( coords.at( i ).first == coord.first - 1 && coords.at(i).second == coord.second ) {
				++neighbors;
			}
			// check if new coord is above existing coord
			if ( coords.at( i ).first == coord.first + 1 && coords.at( i ).second == coord.second ) {
				++neighbors;
			}
			// check if new coord is right of existing coord
			if ( coords.at( i ).first == coord.first && coords.at( i ).second == coord.second - 1 ) {
				++neighbors;
			}
			// check if new coord is left of existing coord
			if ( coords.at( i ).first == coord.first && coords.at( i ).second == coord.second + 1 ) {
				++neighbors;
			}
		}

		if ( !alreadyExists ) {
			perimeter += 4 - neighbors;
			++area;
			coords.emplace_back( coord );
		}
	}

	void add( unsigned int row, unsigned int col ) { add( { row, col } ); }

	void add( std::vector<std::pair<unsigned int, unsigned int>> vec ) {
		for ( std::pair coord : vec ) {
			add( coord );
		}
	}

	void reserve( unsigned int n ) { coords.reserve( n ); }

	void clear() {
		coords.clear();
		area = 0;
		perimeter = 0;
	}

	// implementation only if debugging needed
	/*std::string toString() {
		// new row = old row * 2 + 1
		// new col = old col * 2 + 1
		std::string s;

		return s;
	}*/
};

void dfs( std::pair<unsigned int, unsigned int> coord, char direction, Region& region, MyTGrid<bool>& visited, const MyTGrid<char>& grid );

int main() {
	std::ifstream file( "TestData.txt" );
	MyTGrid<char> grid;
	std::vector<Region> vecRegions;

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

	MyTGrid<bool> visited( grid.getNumColumns() );
	visited.reserve( grid.size() );
	for ( int i = 0; i < grid.getNumRows(); i++ ) {
		visited.addRow( std::vector<bool>( grid.getNumColumns(), false ) );
	}

	for ( int i = 0; i < grid.getNumRows(); i++ ) {
		for ( int j = 0; j < grid.getNumColumns(); j++ ) {
			if ( !visited.at( i, j ) ) {
				vecRegions.emplace_back( Region() );
				if ( i > 0 ) {
					dfs( { i - 1, j }, 'U', vecRegions.back(), visited, grid );
				}
				if ( i < grid.getNumRows() - 1 ) {
					dfs( { i + 1, j }, 'D', vecRegions.back(), visited, grid );
				}
				if ( j > 0 ) {
					dfs( { i, j - 1 }, 'L', vecRegions.back(), visited, grid );
				}
				if ( j < grid.getNumColumns() - 1 ) {
					dfs( { i, j + 1 }, 'R', vecRegions.back(), visited, grid );
				}
			}
		}
	}

	return 0;
}

void dfs( std::pair<unsigned int, unsigned int> coord, char direction, Region& region, MyTGrid<bool>& visited, const MyTGrid<char>& grid ) {
	if ( visited.at( coord.first, coord.second ) ) {
		return;
	}

	switch ( direction ) {
	// check if this letter same as letter below
	case 'U': case 'u':
		if ( grid.peek( coord.first, coord.second ) == grid.peek( coord.first + 1, coord.second ) ) {
			region.add( coord );
			visited.at( coord.first, coord.second ) = true;
			if ( coord.first > 0 ) {
				dfs( { coord.first - 1, coord.second }, 'U', region, visited, grid );
			}
			if ( coord.second > 0 ) {
				dfs( { coord.first, coord.second - 1 }, 'L', region, visited, grid );
			}
			if ( coord.second < grid.getNumColumns() - 1 ) {
				dfs( { coord.first, coord.second + 1 }, 'R', region, visited, grid );
			}
		}
		break;
	// check if this letter same as letter above
	case 'D': case 'd':
		if ( grid.peek( coord.first, coord.second ) == grid.peek( coord.first - 1, coord.second ) ) {
			region.add( coord );
			visited.at( coord.first, coord.second ) = true;
			if ( coord.first < grid.getNumRows() - 1 ) {
				dfs( { coord.first + 1, coord.second }, 'D', region, visited, grid );
			}
			if ( coord.second > 0 ) {
				dfs( { coord.first, coord.second - 1 }, 'L', region, visited, grid );
			}
			if ( coord.second < grid.getNumColumns() - 1 ) {
				dfs( { coord.first, coord.second + 1 }, 'R', region, visited, grid );
			}
		}
		break;
	// check if this letter same as letter right
	case 'L': case 'l':
		if ( grid.peek( coord.first, coord.second ) == grid.peek( coord.first, coord.second + 1 ) ) {
			region.add( coord );
			visited.at( coord.first, coord.second ) = true;
			if ( coord.first > 0 ) {
				dfs( { coord.first - 1, coord.second }, 'U', region, visited, grid );
			}
			if ( coord.first < grid.getNumRows() - 1 ) {
				dfs( { coord.first + 1, coord.second }, 'D', region, visited, grid );
			}
			if ( coord.second > 0 ) {
				dfs( { coord.first, coord.second - 1 }, 'L', region, visited, grid );
			}
		}
		break;
	// check if this letter same as letter left
	case 'R': case 'r':
		if ( grid.peek( coord.first, coord.second ) == grid.peek( coord.first, coord.second - 1 ) ) {
			region.add( coord );
			visited.at( coord.first, coord.second ) = true;
			if ( coord.first > 0 ) {
				dfs( { coord.first - 1, coord.second }, 'U', region, visited, grid );
			}
			if ( coord.first < grid.getNumRows() - 1 ) {
				dfs( { coord.first + 1, coord.second }, 'D', region, visited, grid );
			}
			if ( coord.second < grid.getNumColumns() - 1 ) {
				dfs( { coord.first, coord.second + 1 }, 'R', region, visited, grid );
			}
		}
		break;
	}
}
