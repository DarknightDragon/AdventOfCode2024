#include "MyTGrid.h"
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <utility> // std::pair
#include <algorithm> // std::sort
#include <limits> // std::numeric_limits<unsigned int>

class Region {
private:
	unsigned int perimeter{ 0 };
	std::vector<std::pair<unsigned int, unsigned int>> coords;
	unsigned int minRow{ std::numeric_limits<unsigned int>::max() };
	unsigned int maxRow{ 0 };
	unsigned int minCol{ std::numeric_limits<unsigned int>::max() };
	unsigned int maxCol{ 0 };
	char c;

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

	void checkBounds( unsigned int row, unsigned int col ) {
		if ( minRow > row ) {
			minRow = row;
		}
		if ( maxRow < row ) {
			maxRow = row;
		}
		if ( minCol > col ) {
			minCol = col;
		}
		if ( maxCol < col ) {
			maxCol = col;
		}
	}

	void checkBounds( std::pair<unsigned int, unsigned int> coord ) {
		checkBounds( coord.first, coord.second );
	}

public:
	Region() = default;

	Region( unsigned int row, unsigned int col, char a ) {
		checkBounds( row, col );
		coords.emplace_back( row, col );
		perimeter = 4;
		c = a;
	}

	Region( std::vector<std::pair<unsigned int, unsigned int>> inVec ) {
		std::sort( inVec.begin(), inVec.end() );
		coords.reserve( inVec.size() );
		for ( int i = 0; i < inVec.size(); i++ ) {
			if ( i == 0 ) {
				coords.emplace_back( inVec.at( 0 ) );
				continue;
			}
			if ( inVec.at( i ) != inVec.at( i - 1 ) ) {
				coords.emplace_back( inVec.at( i ) );
			}
		}
		for ( int i = 0; i < coords.size(); i++ ) {
			checkBounds( coords.at( i ) );
			perimeter += 4;
			if ( connectedAbove( coords.at( i ) ) ) { --perimeter; }
			if ( connectedBelow( coords.at( i ) ) ) { --perimeter; }
			if ( connectedLeft( coords.at( i ) ) ) { --perimeter; }
			if ( connectedRight( coords.at( i ) ) ) { --perimeter; }
		}
	}

	char getChar() const { return c; }

	unsigned int getMinRow() const { return minRow; }

	unsigned int getMaxRow() const { return maxRow; }

	unsigned int getMinColumn() const { return minCol; }

	unsigned int getMaxColumn() const { return maxCol; }

	unsigned int getPerimeter() const { return perimeter; }

	unsigned int getArea() const { return coords.size(); }

	unsigned int getPrice() const { return coords.size() * perimeter; }

	unsigned int size() const { return coords.size(); }

	std::pair<unsigned int, unsigned int>& at( unsigned int pos ) { return coords.at( pos ); }
	
	const std::pair<unsigned int, unsigned int>& at( unsigned int pos ) const { return coords.at( pos ); }

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
				neighbors += 2;
			}
			// check if new coord is above existing coord
			if ( coords.at( i ).first == coord.first + 1 && coords.at( i ).second == coord.second ) {
				neighbors += 2;
			}
			// check if new coord is right of existing coord
			if ( coords.at( i ).first == coord.first && coords.at( i ).second == coord.second - 1 ) {
				neighbors += 2;
			}
			// check if new coord is left of existing coord
			if ( coords.at( i ).first == coord.first && coords.at( i ).second == coord.second + 1 ) {
				neighbors += 2;
			}
		}

		if ( !alreadyExists ) {
			perimeter += 4 - neighbors;
			coords.emplace_back( coord );
			checkBounds( coord );
		}
	}

	void add( unsigned int row, unsigned int col ) { add( { row, col } ); }

	void add( std::vector<std::pair<unsigned int, unsigned int>> vec ) {
		for ( std::pair<unsigned int, unsigned int> coord : vec ) {
			add( coord );
		}
	}

	void sort() { std::sort( coords.begin(), coords.end() ); }

	void reserve( unsigned int n ) { coords.reserve( n ); }

	auto begin() { return coords.begin(); }
	
	auto begin() const { return coords.begin(); }

	auto cbegin() const { return coords.cbegin(); }

	auto end() { return coords.end(); }

	auto end() const { return coords.end(); }

	auto cend() const { return coords.cend(); }

	void clear() {
		coords.clear();
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
unsigned int calculateBulkPrices( std::vector<Region>& vecRegions );

int main() {
	std::ifstream file( "Data.txt" );
	MyTGrid<char> grid;
	std::vector<Region> vecRegions;
	unsigned int total{ 0 };

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
				vecRegions.emplace_back( Region( i, j, grid.at( i , j ) ) );
				visited.at( i, j ) = true;
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

	for ( Region& region : vecRegions ) {
		total += region.getPrice();
	}
	std::cout << "Total Price: " << total << std::endl;
	std::cout << "Total Bulk Price: " << calculateBulkPrices( vecRegions ) << std::endl;

	return 0;
}

void dfs( std::pair<unsigned int, unsigned int> coord, char direction, Region& region, MyTGrid<bool>& visited, const MyTGrid<char>& grid ) {
	if ( visited.at( coord.first, coord.second ) ) {
		return;
	}

	switch ( direction ) {
	// check if this letter same as letter below
	case 'U': case 'u':
		if ( grid.at( coord.first, coord.second ) == grid.at( coord.first + 1, coord.second ) ) {
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
		if ( grid.at( coord.first, coord.second ) == grid.at( coord.first - 1, coord.second ) ) {
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
		if ( grid.at( coord.first, coord.second ) == grid.at( coord.first, coord.second + 1 ) ) {
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
		if ( grid.at( coord.first, coord.second ) == grid.at( coord.first, coord.second - 1 ) ) {
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

unsigned int calculateBulkPrices( std::vector<Region>& vecRegions ) {
	unsigned int total{ 0 };

	for ( int i = 0; i < vecRegions.size(); i++ ) {
		Region& region = vecRegions.at( i );
		region.sort();
		unsigned int numSides{ 0 };
		std::vector<std::pair<unsigned int, unsigned int>> sideAbove;
		sideAbove.reserve( region.size() );
		std::vector<std::pair<unsigned int, unsigned int>> sideBelow;
		sideBelow.reserve( region.size() );
		std::vector<std::pair<unsigned int, unsigned int>> sideLeft;
		sideLeft.reserve( region.size() );
		std::vector<std::pair<unsigned int, unsigned int>> sideRight;
		sideRight.reserve( region.size() );
		
		for ( int i = 0; i < region.size(); i++ ) {
			bool isEdge = true;

			// check above
			for ( int j = 0; j < i; j++ ) {
				if ( region.at( i ).first == region.at( j ).first + 1 && region.at( i ).second == region.at( j ).second ) {
					isEdge = false;
					break;
				}
			}
			if ( isEdge ) {
				sideAbove.emplace_back( region.at( i ) );
			}
			else {
				isEdge = true;
			}

			// check below
			for ( int j = i + 1; j < region.size(); j++ ) {
				if ( region.at( i ).first + 1 == region.at( j ).first && region.at( i ).second == region.at( j ).second ) {
					isEdge = false;
					break;
				}
				if ( region.at( i ).first + 1 < region.at( j ).first ) {
					break;
				}
			}
			if ( isEdge ) {
				sideBelow.emplace_back( region.at( i ) );
			}
			else {
				isEdge = true;
			}

			// check left
			if ( i == 0 || region.at( i ).first != region.at( i - 1 ).first || region.at( i ).second != region.at( i - 1 ).second + 1 ) {
				sideLeft.emplace_back( region.at( i ) );
			}

			// check right
			if ( i == region.size() - 1 || region.at( i ).first != region.at( i + 1 ).first || region.at( i ).second != region.at( i + 1 ).second - 1 ) {
				sideRight.emplace_back( region.at( i ) );
			}
		} // close for each coord

		// find all sides above
		++numSides;
		for ( int i = 1; i < sideAbove.size(); i++ ) {
			if ( sideAbove.at( i ).first != sideAbove.at( i - 1 ).first || sideAbove.at( i ).second != sideAbove.at( i - 1 ).second + 1 ) {
				++numSides;
			}
		}

		// find all sides below
		++numSides;
		for ( int i = 1; i < sideBelow.size(); i++ ) {
			if ( sideBelow.at( i ).first != sideBelow.at( i - 1 ).first || sideBelow.at( i ).second != sideBelow.at( i - 1 ).second + 1 ) {
				++numSides;
			}
		}

		// find all sides left
		++numSides;
		// lambda to sort a pair based on the second element, stable to keep relative x sorted order
		std::stable_sort( sideLeft.begin(), sideLeft.end(), []( auto& left, auto& right ) { return left.second < right.second; } );
		for ( int i = 1; i < sideLeft.size(); i++ ) {
			if ( sideLeft.at( i ).second != sideLeft.at( i - 1 ).second || sideLeft.at( i ).first != sideLeft.at( i - 1 ).first + 1 ) {
				++numSides;
			}
		}

		// find all sides right
		++numSides;
		// lambda to sort a pair based on the second element, stable to keep relative x sorted order
		std::stable_sort( sideRight.begin(), sideRight.end(), []( auto& left, auto& right ) { return left.second < right.second; } );
		for ( int i = 1; i < sideRight.size(); i++ ) {
			if ( sideRight.at( i ).second != sideRight.at( i - 1 ).second || sideRight.at( i ).first != sideRight.at( i - 1 ).first + 1 ) {
				++numSides;
			}
		}

		total += numSides * region.getArea();
	} // close for each region

	return total;
}