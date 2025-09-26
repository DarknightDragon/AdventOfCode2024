#include "MyTGrid.h"
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
// Part 2
#include <utility> // std::pair
#include <map> // ordered std::multimap
#include <chrono> // used to benchmark time performance

int countChar( const MyTGrid<char>& grid, char c = 'X' );
void runPatrolRoute( MyTGrid<char>& grid, int guardRow, int guardCol, char guardPos );
std::vector<std::pair<int, int>> findAllObstructionLoops( MyTGrid<char>& grid, int guardRow, int guardCol, char guardPos, char c = 'X' );
bool isLoop( const MyTGrid<char>& grid, int guardRow, int guardCol, char guardPos, int obstructionRow, int obstructionCol );

int main() {
	std::ifstream file( "Data.txt" );
	MyTGrid<char> grid;
	unsigned int distinctPos { 0 };
	unsigned int guardRow { 0 };
	unsigned int guardCol { 0 };
	char guardPos {};
	// Part 2
	std::vector<std::pair<int, int>> obstructionLoopCoords;

	if ( !file.is_open() ) {
		std::cerr << "File error" << std::endl;
		return 1;
	}

	// populate grid
	for ( std::string line; std::getline( file, line ); ) {
		std::vector<char> temp;
		temp.reserve( line.length() );

		int currCol { 0 };
		for ( char c : line ) {
			switch ( c ) {
			case '^':
				guardRow = grid.getNumRows();
				guardCol = currCol;
				guardPos = 'U';
				break;
			case '>':
				guardRow = grid.getNumRows();
				guardCol = currCol;
				guardPos = 'R';
				break;
			case 'v':
				guardRow = grid.getNumRows();
				guardCol = currCol;
				guardPos = 'D';
				break;
			case '<':
				guardRow = grid.getNumRows();
				guardCol = currCol;
				guardPos = 'L';
				break;
			}
			temp.push_back( c );
			++currCol;
		}
		grid.addRow( temp );
	} // close populate grid

	file.close();

	auto start = std::chrono::high_resolution_clock::now();
	// generate path with 'X's
	runPatrolRoute( grid, guardRow, guardCol, guardPos );
	
	//// speed test for find all possible loops repeating a number of times
	//for ( int i = 0; i < 10; i++ ) {
	//	auto start = std::chrono::high_resolution_clock::now();
	//	// Part 2
	//	// use path and for each X, replace with an obstacle and check if loop
	//	obstructionLoopCoords = findAllObstructionLoops( grid, guardRow, guardCol, guardPos );
	//	auto duration = std::chrono::duration_cast<std::chrono::milliseconds>( std::chrono::high_resolution_clock::now() - start );
	//	std::cout << "TIME TO RUN: " << duration.count() << " MILLISECONDS." << std::endl;
	//}
	
	// Part 2
	// use path and for each X, replace with an obstacle and check if loop
	obstructionLoopCoords = findAllObstructionLoops( grid, guardRow, guardCol, guardPos );
	auto duration = std::chrono::duration_cast<std::chrono::milliseconds>( std::chrono::high_resolution_clock::now() - start );
	std::cout << "TIME TO RUN: " << duration.count() << " MILLISECONDS." << std::endl;

	std::cout << "Number of distinct positions guard will visit before leaving area: " << countChar( grid ) << "\n";
	std::cout << "Number of different positions choosable for a new obstruction: " << obstructionLoopCoords.size() << std::endl;

	return 0;
}

int countChar( const MyTGrid<char>& grid, char c )
{
	int count { 0 };
	int numRows = grid.getNumRows();
	int numCols = grid.getNumColumns();

	for ( int i = 0; i < numRows; i++ ) {
		for ( int j = 0; j < numCols; j++ ) {
			if ( grid.at( i, j ) == c ) {
				count++;
			}
		}
	}

	return count;
}

void runPatrolRoute( MyTGrid<char>& grid, int guardRow, int guardCol, char guardPos )
{
	int maxRow = grid.getNumRows();
	int maxCol = grid.getNumColumns();
	while ( guardRow != -1 && guardRow != maxRow && guardCol != -1 && guardCol != maxCol ) {
		grid.at( guardRow, guardCol ) = 'X';

		// make sure guard location doesn't become out of bounds
		if ( ( guardRow == 0 && guardPos == 'U' ) ||
			( guardRow == maxRow - 1 && guardPos == 'D' ) ||
			( guardCol == 0 && guardPos == 'L' ) ||
			( guardCol == maxCol - 1 && guardPos == 'R' ) ) {
			break;
		}

		switch ( guardPos ) {
		case 'U':
			if ( grid.at( guardRow - 1, guardCol ) == '#' ) {
				guardPos = 'R';
			}
			else {
				guardRow--;
			}
			break;
		case 'R':
			if ( grid.at( guardRow, guardCol + 1 ) == '#' ) {
				guardPos = 'D';
			}
			else {
				guardCol++;
			}
			break;
		case 'D':
			if ( grid.at( guardRow + 1, guardCol ) == '#' ) {
				guardPos = 'L';
			}
			else {
				guardRow++;
			}
			break;
		case 'L':
			if ( grid.at( guardRow, guardCol - 1 ) == '#' ) {
				guardPos = 'U';
			}
			else {
				guardCol--;
			}
			break;
		}
	}
}

std::vector<std::pair<int, int>> findAllObstructionLoops( MyTGrid<char>& grid, int guardRow, int guardCol, char guardPos, char c ) {
	std::vector<std::pair<int, int>> coords;

	unsigned int maxRow{ grid.getNumRows() };
	unsigned int maxCol{ grid.getNumColumns() };
	std::map<std::pair<int, int>, bool> visited;
	//int currStep{ 0 };
	while ( guardRow != -1 && guardRow != maxRow && guardCol != -1 && guardCol != maxCol ) {
		grid.at( guardRow, guardCol ) = c;

		// make sure guard location doesn't become out of bounds
		if ( ( guardRow == 0 && guardPos == 'U' ) ||
			( guardRow == maxRow - 1 && guardPos == 'D' ) ||
			( guardCol == 0 && guardPos == 'L' ) ||
			( guardCol == maxCol - 1 && guardPos == 'R' ) ) {
			break;
		}

		switch ( guardPos ) {
		case 'U':
			if ( grid.at( guardRow - 1, guardCol ) == '#' ) {
				guardPos = 'R';
			}
			else {
				if ( visited.find( { guardRow - 1, guardCol } ) == visited.end() ) {
					visited.insert( { { guardRow - 1, guardCol }, true } );
					grid.at( guardRow - 1, guardCol ) = '#';
					if ( isLoop( grid, guardRow, guardCol, guardPos, guardRow - 1, guardCol ) ) {
						//coords.emplace_back( guardRow - 1, guardCol );
						coords.push_back( { guardRow - 1, guardCol } );
					}
					grid.at( guardRow - 1, guardCol ) = c;
				}
				//currStep++;
				guardRow--;
			}
			break;
		case 'R':
			if ( grid.at( guardRow, guardCol + 1 ) == '#' ) {
				guardPos = 'D';
			}
			else {
				if ( visited.find( { guardRow, guardCol + 1 } ) == visited.end() ) {
					visited.insert( { { guardRow, guardCol + 1 }, true } );
					grid.at( guardRow, guardCol + 1 ) = '#';
					if ( isLoop( grid, guardRow, guardCol, guardPos, guardRow, guardCol + 1 ) ) {
						//coords.emplace_back( guardRow, guardCol + 1 );
						coords.push_back( { guardRow, guardCol + 1 } );
					}
					grid.at( guardRow, guardCol + 1 ) = c;
				}
				//currStep++;
				guardCol++;
			}
			break;
		case 'D':
			if ( grid.at( guardRow + 1, guardCol ) == '#' ) {
				guardPos = 'L';
			}
			else {
				if ( visited.find( { guardRow + 1, guardCol } ) == visited.end() ) {
					visited.insert( { { guardRow + 1, guardCol }, true } );
					grid.at( guardRow + 1, guardCol ) = '#';
					if ( isLoop( grid, guardRow, guardCol, guardPos, guardRow + 1, guardCol ) ) {
						//coords.emplace_back( guardRow + 1, guardCol );
						coords.push_back( { guardRow + 1, guardCol } );
					}
					grid.at( guardRow + 1, guardCol ) = c;
				}
				//currStep++;
				guardRow++;
			}
			break;
		case 'L':
			if ( grid.at( guardRow, guardCol - 1 ) == '#' ) {
				guardPos = 'U';
			}
			else {
				if ( visited.find( { guardRow, guardCol - 1 } ) == visited.end() ) {
					visited.insert( { { guardRow, guardCol - 1 }, true } );
					grid.at( guardRow, guardCol - 1 ) = '#';
					if ( isLoop( grid, guardRow, guardCol, guardPos, guardRow, guardCol - 1 ) ) {
						//coords.emplace_back( guardRow, guardCol - 1 );
						coords.push_back( { guardRow, guardCol - 1 } );
					}
					grid.at( guardRow, guardCol - 1 ) = c;
				}
				//currStep++;
				guardCol--;
			}
			break;
		}
		//std::cout << "PATH COUNT: " << currStep << " out of " << pathCount << std::endl;
	}

	return coords;
}

bool isLoop( const MyTGrid<char>& grid, int guardRow, int guardCol, char guardPos, int obstructionRow, int obstructionCol ) {
	bool loop { false };
	std::multimap<std::pair<int, int>, char> visitedLocations;
	const auto endItr = visitedLocations.end();

	// run patrol loop checking to make sure not in loop
	int maxRow = grid.getNumRows();
	int maxCol = grid.getNumColumns();
	while ( guardRow != -1 && guardRow != maxRow && guardCol != -1 && guardCol != maxCol ) {
		auto guardCoord = std::make_pair( guardRow, guardCol );
		// check curr position if previously visited before moving on
		if ( visitedLocations.find( guardCoord ) != endItr ) {
			for ( auto [itr, rangeEnd] = visitedLocations.equal_range( guardCoord ); itr != rangeEnd; itr++ ) {
				if ( itr->second == guardPos ) {
					loop = true;
					break;
				}
			}
		}
		if ( loop ) {
			break;
		}

		visitedLocations.insert( { guardCoord, guardPos } );

		// make sure guard location doesn't become out of bounds
		if ( ( guardRow == 0 && guardPos == 'U' ) ||
			( guardRow == maxRow - 1 && guardPos == 'D' ) ||
			( guardCol == 0 && guardPos == 'L' ) ||
			( guardCol == maxCol - 1 && guardPos == 'R' ) ) {
			break;
		}

		switch ( guardPos ) {
		case 'U':
			if ( grid.at( guardRow - 1, guardCol ) == '#' ) {
				guardPos = 'R';
			}
			else {
				guardRow--;
			}
			break;
		case 'R':
			if ( grid.at( guardRow, guardCol + 1 ) == '#' ) {
				guardPos = 'D';
			}
			else {
				guardCol++;
			}
			break;
		case 'D':
			if ( grid.at( guardRow + 1, guardCol ) == '#' ) {
				guardPos = 'L';
			}
			else {
				guardRow++;
			}
			break;
		case 'L':
			if ( grid.at( guardRow, guardCol - 1 ) == '#' ) {
				guardPos = 'U';
			}
			else {
				guardCol--;
			}
			break;
		}
	}

	return loop;
}
