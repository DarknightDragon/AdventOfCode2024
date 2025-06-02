#include "MyTGrid.h"
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <utility> // std::pair
#include <unordered_map> // std::unordered_multimap and std::unordered_map

int generateAntinodeLocsP1( MyTGrid<char> grid, std::unordered_multimap<char, std::pair<unsigned int, unsigned int>>& map, std::unordered_map<char, int>& keyMap );
int generateAntinodeLocsP2( MyTGrid<char> grid, std::unordered_multimap<char, std::pair<unsigned int, unsigned int>>& map, std::unordered_map<char, int>& keyMap );

int main() {
	std::ifstream file( "Data.txt" );
	MyTGrid<char> grid;
	unsigned int totalUniqueAntinodeLocsP1{ 0 }, totalUniqueAntinodeLocsP2{ 0 };
	// map all characters and coords (row,col)
	std::unordered_multimap<char, std::pair<unsigned int, unsigned int>> map;
	// map each unique key and how many times it appears
	std::unordered_map<char, int> keyMap;

	if ( !file.is_open() ) {
		std::cerr << "File error" << std::endl;
		return 1;
	}

	for ( std::string line; std::getline( file, line ); ) {
		std::vector<char> temp;
		temp.reserve( line.length() );
		for ( unsigned int i = 0; i < line.length(); i++ ) {
			char c{ line[i] };
			temp.emplace_back( c );
			if ( c != '.' ) {
				map.emplace( c, std::make_pair( grid.getNumRows(), i ) );
				if ( keyMap.find( c ) == keyMap.end() ) {
					keyMap.emplace( c, 1 );
				}
				else {
					keyMap.at( c )++;
				}
			}
		}
		grid.addRow( temp );
	}

	file.close();

	totalUniqueAntinodeLocsP1 = generateAntinodeLocsP1( grid, map, keyMap );
	totalUniqueAntinodeLocsP2 = generateAntinodeLocsP2( grid, map, keyMap );

	std::cout << "Number of unique antinode locations P1: " << totalUniqueAntinodeLocsP1 <<
		"\nNumber of unique antinode locations P2: " << totalUniqueAntinodeLocsP2 << std::endl;

	return 0;
}

int generateAntinodeLocsP1( MyTGrid<char> grid, std::unordered_multimap<char, std::pair<unsigned int, unsigned int>>& map, std::unordered_map<char, int>& keyMap ) {
	int totalUniqueAntinodeLocs{ 0 };

	// find each key and the number of appearances. make a counter, for each key in map, compare against each other key. increate iterator for each counter to not redo matches
	// check based on slope between each match. check if grid at location is #, if not increase totalUniqueAntinode and make char #
	for ( auto entry : keyMap ) {
		if ( entry.second == 1 ) {
			continue;
		}
		for ( auto [itr, endItr] = map.equal_range( entry.first ); itr != endItr; itr++ ) {
			auto nextItr = itr;
			unsigned int startRow = itr->second.first, startCol = itr->second.second;
			while ( ++nextItr != endItr ) {
				unsigned int nextRow = nextItr->second.first, nextCol = nextItr->second.second;
				int deltaRow = nextRow - startRow, deltaCol = nextCol - startCol;
				if ( startRow - deltaRow >= 0 && startCol - deltaCol >= 0 &&
					 startRow - deltaRow < grid.getNumRows() && startCol - deltaCol < grid.getNumColumns() &&
					 grid.at( startRow - deltaRow, startCol - deltaCol ) != '#' ) {
					grid.at( startRow - deltaRow, startCol - deltaCol ) = '#';
					++totalUniqueAntinodeLocs;
				}
				if ( nextRow + deltaRow >= 0 && nextCol + deltaCol >= 0 &&
					 nextRow + deltaRow < grid.getNumRows() && nextCol + deltaCol < grid.getNumColumns() &&
					 grid.at( nextRow + deltaRow, nextCol + deltaCol ) != '#' ) {
					grid.at( nextRow + deltaRow, nextCol + deltaCol ) = '#';
					++totalUniqueAntinodeLocs;
				}
			}
		}
	}

	return totalUniqueAntinodeLocs;
}

int generateAntinodeLocsP2( MyTGrid<char> grid, std::unordered_multimap<char, std::pair<unsigned int, unsigned int>>& map, std::unordered_map<char, int>& keyMap ) {
	int totalUniqueAntinodeLocs{ 0 };

	// find each key and the number of appearances. make a counter, for each key in map, compare against each other key. increate iterator for each counter to not redo matches
	// check based on slope between each match. check if grid at location is #, if not increase totalUniqueAntinode and make char #
	for ( auto entry : keyMap ) {
		if ( entry.second == 1 ) {
			continue;
		}
		for ( auto [itr, endItr] = map.equal_range( entry.first ); itr != endItr; itr++ ) {
			auto nextItr = itr;
			unsigned int startRow = itr->second.first, startCol = itr->second.second;
			if ( grid.at( startRow, startCol ) != '#' ) {
				grid.at( startRow, startCol ) = '#';
				++totalUniqueAntinodeLocs;
			}
			while ( ++nextItr != endItr ) {
				unsigned int nextRow = nextItr->second.first, nextCol = nextItr->second.second;
				int deltaRow = nextRow - startRow, deltaCol = nextCol - startCol;
				if ( grid.at( nextRow, nextCol ) != '#' ) {
					grid.at( nextRow, nextCol ) = '#';
					++totalUniqueAntinodeLocs;
				}
				for ( int scalar = 1, scalarLimit = grid.getNumRows() <= grid.getNumColumns() ? grid.getNumRows() : grid.getNumColumns(); scalar < scalarLimit; scalar++ ) {
					if ( startRow - scalar * deltaRow >= 0 && startCol - scalar * deltaCol >= 0 &&
						 startRow - scalar * deltaRow < grid.getNumRows() && startCol - scalar * deltaCol < grid.getNumColumns() &&
						 grid.at( startRow - scalar * deltaRow, startCol - scalar * deltaCol) != '#' ) {
						grid.at( startRow - scalar * deltaRow, startCol - scalar * deltaCol ) = '#';
						++totalUniqueAntinodeLocs;
					}
					if ( nextRow + scalar * deltaRow >= 0 && nextCol + scalar * deltaCol >= 0 &&
						 nextRow + scalar * deltaRow < grid.getNumRows() && nextCol + scalar * deltaCol < grid.getNumColumns() &&
						 grid.at( nextRow + scalar * deltaRow, nextCol + scalar * deltaCol ) != '#' ) {
						grid.at( nextRow + scalar * deltaRow, nextCol + scalar * deltaCol ) = '#';
						++totalUniqueAntinodeLocs;
					}
				}
			}
		}
	}

	return totalUniqueAntinodeLocs;
}