//#include <iostream>
//#include <fstream>
//#include <string>
//#include <vector>
//#include "MyTGrid.h"
//// Part 2
//#include <utility> // std::pair
//#include <map>
//#include <chrono>
//
//int countChar( const MyTGrid<char>& grid, char c = 'X' );
//void runPatrolRoute( MyTGrid<char>& grid, int guardRow, int guardCol, char guardPos );
//std::vector<std::pair<int, int>> findAllObstructionLoops( MyTGrid<char>& grid, int guardRow, int guardCol, char guardPos, char c = 'X' );
//bool isLoop( const MyTGrid<char>& grid, int guardRow, int guardCol, char guardPos, int obstructionRow, int obstructionCol );
//
//int main() {
//	std::ifstream file( "Data.txt" );
//	MyTGrid<char> grid;
//	unsigned int distinctPos{ 0 };
//	unsigned int guardRow{ 0 };
//	unsigned int guardCol{ 0 };
//	char guardPos{};
//	// Part 2
//	std::vector<std::pair<int, int>> obstructionLoopCoords;
//
//	if ( !file.is_open() ) {
//		std::cerr << "File error" << std::endl;
//		return 1;
//	}
//
//	// populate grid
//	for ( std::string line; std::getline( file, line ); ) {
//		std::vector<char> temp;
//		temp.reserve( line.length() );
//
//		int currCol{ 0 };
//		for ( char c : line ) {
//			switch ( c ) {
//			case '^':
//				guardRow = grid.getNumRows();
//				guardCol = currCol;
//				guardPos = 'U';
//				break;
//			case '>':
//				guardRow = grid.getNumRows();
//				guardCol = currCol;
//				guardPos = 'R';
//				break;
//			case 'v':
//				guardRow = grid.getNumRows();
//				guardCol = currCol;
//				guardPos = 'D';
//				break;
//			case '<':
//				guardRow = grid.getNumRows();
//				guardCol = currCol;
//				guardPos = 'L';
//				break;
//			}
//			temp.push_back( c );
//			currCol++;
//		}
//		grid.addRow( temp );
//	} // close populate grid
//
//	file.close();
//	//std::ofstream out( "TEST.txt" ); grid.at( 15, 112 ) = 'O'; out << grid << std::endl; out.close(); grid.at( 15, 112 ) = 'X';
//
//	auto start = std::chrono::high_resolution_clock::now();
//	// generate path with 'X's
//	runPatrolRoute( grid, guardRow, guardCol, guardPos );
//
//	//for ( int i = 0; i < 100; i++ ) {
//	//	auto start = std::chrono::high_resolution_clock::now();
//	//	// Part 2
//	//	// use path and for each X, replace with an obstacle and check if loop
//	//	obstructionLoopCoords = findAllObstructionLoops( grid, guardRow, guardCol, guardPos );
//	//	auto duration = std::chrono::duration_cast<std::chrono::milliseconds>( std::chrono::high_resolution_clock::now() - start );
//	//	std::cout << "TIME TO RUN: " << duration.count() << " MILLISECONDS." << std::endl;
//	//}
//	
//	// Part 2
//	// use path and for each X, replace with an obstacle and check if loop
//	obstructionLoopCoords = findAllObstructionLoops( grid, guardRow, guardCol, guardPos, 'X' );
//	auto duration = std::chrono::duration_cast<std::chrono::milliseconds>( std::chrono::high_resolution_clock::now() - start );
//	std::cout << "TIME TO RUN: " << duration.count() << " MILLISECONDS." << std::endl;
//
//	std::cout << "Number of distinct positions guard will visit before leaving area: " << countChar( grid ) << "\n";
//	std::cout << "Number of different positions choosable for a new obstruction: " << obstructionLoopCoords.size() << std::endl;
//
//	return 0;
//}
//
//int countChar( const MyTGrid<char>& grid, char c )
//{
//	int count{ 0 };
//	int numRows = grid.getNumRows();
//	int numCols = grid.getNumColumns();
//
//	for ( int i = 0; i < numRows; i++ ) {
//		for ( int j = 0; j < numCols; j++ ) {
//			if ( grid.peek( i, j ) == c ) {
//				count++;
//			}
//		}
//	}
//
//	return count;
//}
//
//void runPatrolRoute( MyTGrid<char>& grid, int guardRow, int guardCol, char guardPos )
//{
//	int maxRow = grid.getNumRows();
//	int maxCol = grid.getNumColumns();
//	while ( guardRow != -1 && guardRow != maxRow && guardCol != -1 && guardCol != maxCol ) {
//		grid.at( guardRow, guardCol ) = 'X';
//
//		// make sure guard location doesn't become out of bounds
//		if ( ( guardRow == 0 && guardPos == 'U' ) ||
//			( guardRow == maxRow - 1 && guardPos == 'D' ) ||
//			( guardCol == 0 && guardPos == 'L' ) ||
//			( guardCol == maxCol - 1 && guardPos == 'R' ) ) {
//			break;
//		}
//
//		switch ( guardPos ) {
//		case 'U':
//			if ( grid.at( guardRow - 1, guardCol ) == '#' ) {
//				guardPos = 'R';
//			}
//			else {
//				guardRow-- ;
//			}
//			break;
//		case 'R':
//			if ( grid.at( guardRow, guardCol + 1 ) == '#' ) {
//				guardPos = 'D';
//			}
//			else {
//				guardCol++;
//			}
//			break;
//		case 'D':
//			if ( grid.at( guardRow + 1, guardCol ) == '#' ) {
//				guardPos = 'L';
//			}
//			else {
//				guardRow++;
//			}
//			break;
//		case 'L':
//			if ( grid.at( guardRow, guardCol - 1 ) == '#' ) {
//				guardPos = 'U';
//			}
//			else {
//				guardCol--;
//			}
//			break;
//		}
//	}
//}
//
//std::vector<std::pair<int, int>> findAllObstructionLoops( MyTGrid<char>& grid, int guardRow, int guardCol, char guardPos, char c )
//{
//	std::vector<std::pair<int, int>> coords;
//
//	int maxRow = grid.getNumRows();
//	int maxCol = grid.getNumColumns();
//	for ( int i = 0; i < maxRow; i++ ) {
//		for ( int j = 0; j < maxCol; j++ ) {
//			if ( grid.at( i, j ) != 'X' ) {
//				continue;
//			}
//
//			grid.at( i, j ) = '#';
//			//std::cout << "LOOP AT: (" << i << ", " << j << " )" << std::endl;
//			if ( isLoop( grid, guardRow, guardCol, guardPos, i, j ) ) {
//				coords.push_back( std::make_pair( i, j ) );
//			}
//			grid.at( i, j ) = 'X';
//		}
//		//std::cout << "LOOP LINE AT: " << i << " OUT OF " << grid.getNumRows() << std::endl;
//	}
//
//	return coords;
//}
//
//bool isLoop( const MyTGrid<char>& grid, int guardRow, int guardCol, char guardPos, int obstructionRow, int obstructionCol )
//{
//	// hasVisited from POV of obstruction
//	bool loop { false };
//	std::multimap<std::pair<int, int>, char> visitedLocations;
//	const auto endItr = visitedLocations.end();
//	//bool hasVisitedBelow { false }, hasVisitedRight { false }, hasVisitedAbove { false }, hasVisitedLeft { false };
//
//	// run patrol loop checking to make sure not in loop
//	int maxRow = grid.getNumRows();
//	int maxCol = grid.getNumColumns();
//	while ( guardRow != -1 && guardRow != maxRow && guardCol != -1 && guardCol != maxCol ) {
//		auto guardCoord = std::make_pair( guardRow, guardCol );
//		// check curr position if previously visited before moving on
//		/*if ( guardRow - 1 == obstructionRow && guardCol == obstructionCol && guardPos == 'U' ) {
//			if ( hasVisitedBelow ) {
//				loop = true;
//				break;
//			}
//			hasVisitedBelow = true;
//		}
//		else if ( guardRow == obstructionRow && guardCol - 1 == obstructionCol && guardPos == 'L' ) {
//			if ( hasVisitedRight ) {
//				loop = true;
//				break;
//			}
//			hasVisitedRight = true;
//		}
//		else if ( guardRow + 1 == obstructionRow && guardCol == obstructionCol && guardPos == 'D' ) {
//			if ( hasVisitedAbove ) {
//				loop = true;
//				break;
//			}
//			hasVisitedAbove = true;
//		}
//		else if ( guardRow == obstructionRow && guardCol + 1 == obstructionCol && guardPos == 'R' ) {
//			if ( hasVisitedLeft ) {
//				loop = true;
//				break;
//			}
//			hasVisitedLeft = true;
//		}*/
//		if ( visitedLocations.find( guardCoord ) != endItr ) {
//			for ( auto [itr, rangeEnd] = visitedLocations.equal_range( guardCoord ); itr != rangeEnd; itr++ ) {
//				if ( itr->second == guardPos ) {
//					//// This is the first instance where obstruction added creates a loop that does not directly loop on the obstruction
//					//if ( obstructionRow == 15 && obstructionCol == 112 ) { std::cout << "ROW: " << guardRow << "\tCOL: " << guardCol << "\tPOS: " << guardPos << std::endl; }
//					loop = true;
//					break;
//				}
//			}
//		}
//		if ( loop ) {
//			break;
//		}
//
//		visitedLocations.insert( { guardCoord, guardPos } );
//
//		// make sure guard location doesn't become out of bounds
//		if ( ( guardRow == 0 && guardPos == 'U' ) ||
//			( guardRow == maxRow - 1 && guardPos == 'D' ) ||
//			( guardCol == 0 && guardPos == 'L' ) ||
//			( guardCol == maxCol - 1 && guardPos == 'R' ) ) {
//			break;
//		}
//
//		switch ( guardPos ) {
//		case 'U':
//			if ( grid.peek( guardRow - 1, guardCol ) == '#' ) {
//				guardPos = 'R';
//			}
//			else {
//				guardRow--;
//			}
//			break;
//		case 'R':
//			if ( grid.peek( guardRow, guardCol + 1 ) == '#' ) {
//				guardPos = 'D';
//			}
//			else {
//				guardCol++;
//			}
//			break;
//		case 'D':
//			if ( grid.peek( guardRow + 1, guardCol ) == '#' ) {
//				guardPos = 'L';
//			}
//			else {
//				guardRow++;
//			}
//			break;
//		case 'L':
//			if ( grid.peek( guardRow, guardCol - 1 ) == '#' ) {
//				guardPos = 'U';
//			}
//			else {
//				guardCol--;
//			}
//			break;
//		}
//	}
//
//	return loop;
//}
