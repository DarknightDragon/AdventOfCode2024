#include "MyTGrid.h"
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <unordered_map> // std::unordered_multimap
#include <utility> // std::pair

void initP1Variables( const MyTGrid<int>& grid, std::vector<std::vector<int>>& adjList, std::vector<int>& trailheadLocs );
std::pair<unsigned int, unsigned int> generateCompleteTrails( const MyTGrid<int>& grid, const std::vector<std::vector<int>>& adjList, const std::vector<int>& trailheadLocs );
bool completeTrail( const MyTGrid<int>& grid, const std::vector<std::vector<int>>& adjList, std::unordered_multimap<int, int>& visited, int startPos, int pos, unsigned int& sum, unsigned int& rating );

int main() {
	std::ifstream file( "Data.txt" ); // input is only digits
	MyTGrid<int> grid;
	std::vector<std::vector<int>> adjList;
	adjList.reserve( 1937 ); // number of elems in data + 1
	std::vector<int> trailheadLocs;
	trailheadLocs.reserve( 500 ); // sufficiently large number to cut down on reallocs

	if ( !file.is_open() ) {
		std::cerr << "File error" << std::endl;
		return 1;
	}

	for ( std::string line; std::getline( file, line ); ) {
		std::vector<int> temp;
		temp.reserve( line.length() );
		for ( char c : line ) {
			temp.emplace_back( c - '0' );
		}
		grid.addRow( temp );
	}

	file.close();

	initP1Variables( grid, adjList, trailheadLocs );

	std::pair<unsigned int, unsigned int> ans = generateCompleteTrails( grid, adjList, trailheadLocs );
	std::cout << "Total score sum: " << ans.first << "\nTotal rating sum: " << ans.second << std::endl;

	return 0;
}

void initP1Variables( const MyTGrid<int>& grid, std::vector<std::vector<int>>& adjList, std::vector<int>& trailheadLocs ) {
	adjList = std::vector<std::vector<int>>( grid.size() + 1, std::vector<int>() );
	for ( int i = 0; i < grid.getNumRows(); i++ ) {
		for ( int j = 0; j < grid.getNumColumns(); j++ ) {
			// create trailhead (0 locations) array
			if ( grid.at( i, j ) == 0 ) {
				trailheadLocs.emplace_back( i * grid.getNumColumns() + j );
			}
			// check number left
			if ( j > 0 ) {
				if ( grid.at( i, j - 1 ) == grid.at( i, j ) + 1 ) {
					adjList[i * grid.getNumColumns() + j].emplace_back( i * grid.getNumColumns() + j - 1 );
				}
			}
			// check number right
			if ( j < grid.getNumColumns() - 1 ) {
				if ( grid.at( i, j + 1 ) == grid.at( i, j ) + 1 ) {
					adjList[i * grid.getNumColumns() + j].emplace_back( i * grid.getNumColumns() + j + 1 );
				}
			}
			// check number above
			if ( i > 0 ) {
				if ( grid.at( i - 1, j ) == grid.at( i, j ) + 1 ) {
					adjList[i * grid.getNumColumns() + j].emplace_back( ( i - 1 ) * grid.getNumColumns() + j );
				}
			}
			// check number below
			if ( i < grid.getNumRows() - 1 ) {
				if ( grid.at( i + 1, j ) == grid.at( i, j ) + 1 ) {
					adjList[i * grid.getNumColumns() + j].emplace_back( ( i + 1 ) * grid.getNumColumns() + j );
				}
			}
		}
	}
}

std::pair<unsigned int, unsigned int> generateCompleteTrails( const MyTGrid<int>& grid, const std::vector<std::vector<int>>& adjList, const std::vector<int>& trailheadLocs ) {
	unsigned int sum{ 0 }, rating{ 0 };
	std::unordered_multimap<int, int> visited;
	
	for ( int pos : trailheadLocs ) {
		if ( adjList[pos].empty() ) {
			continue;
		}

		//// iterative
		//int row = pos / grid.getNumRows(), col = pos - ( row * grid.getNumRows() );
		//int nextPos = pos, int index = 0;
		//while ( grid.at( row, col ) != 9 ) {
		//	if ( index )
		//}

		// recursive
		completeTrail( grid, adjList, visited, pos, pos, sum, rating );
	}
	
	return std::make_pair(sum, rating);
}

bool completeTrail( const MyTGrid<int>& grid, const std::vector<std::vector<int>>& adjList, std::unordered_multimap<int, int>& visited, int startPos, int pos, unsigned int& sum, unsigned int& rating ) {
	int row = pos / grid.getNumRows(), col = pos - ( row * grid.getNumRows() );
	if ( grid.at( row, col ) == 9 ) {
		++rating;
		if ( visited.find( startPos ) == visited.end() ) {
			visited.emplace( startPos, pos );
			++sum;
		}
		else {
			auto [itr, endItr] = visited.equal_range( startPos );
			while ( itr != endItr ) {
				if ( itr->second == pos ) {
					break;
				}
				++itr;
			}
			if ( itr == endItr ) {
				visited.emplace( startPos, pos );
				++sum;
			}
		}
		return true;
	}
	std::vector<int> edgeList = adjList[pos];
	if ( edgeList.empty() ) {
		return false;
	}
	for ( int e : edgeList ) {
		completeTrail( grid, adjList, visited, startPos, e, sum, rating );
	}
}
