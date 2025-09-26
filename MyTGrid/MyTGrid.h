#pragma once
#ifndef MY_T_GRID_FILE_H
#define MY_T_GRID_FILE_H

#include <vector>
#include <iostream>
#include <stdexcept>

template <typename T> class MyTGrid {
private:
	unsigned int numCols{ 0 };
	unsigned int numRows{ 0 };
	std::vector<T> grid;

public:
	MyTGrid() = default;

	MyTGrid( unsigned int cols ) {
		if ( cols == 0 ) {
			throw std::invalid_argument( "Grid constructor has 0 size." );
		}
		numCols = cols;
		grid.reserve( cols );
	}

	template <typename Container>
	MyTGrid( unsigned int cols, Container data ) {
		if ( cols == 0 ) {
			throw std::invalid_argument( "Grid constructor has 0 size." );
		}
		if ( data.size() % cols != 0 ) {
			throw std::invalid_argument( "Grid constructor has invalid vector size that does not match column size." );
		}

		numCols = cols;
		numRows = data.size() / cols;
		for ( auto elem : data ) {
			grid.emplace_back( elem );
		}
	}

	unsigned int size() const { return grid.size(); }

	unsigned int getNumColumns() const { return numCols; }

	unsigned int getNumRows() const { return numRows; }

	void setNumColumns( unsigned int col ) {
		if ( col == 0 ) {
			throw std::invalid_argument( "Received non-positive column value." );
		}
		if ( grid.size() % col != 0 ) {
			throw std::invalid_argument( "Current grid size does not support new column value. Violates rectangle grid." );
		}

		numCols = col;
		numRows = grid.size() / col;
	}

	template <typename Container>
	void addRow( Container input ) {
		if ( numCols == 0 ) {
			numCols = input.size();
		}
		else if ( input.size() != numCols ) {
			throw std::length_error( "Input size differs from current column size. Violates rectangle grid." );
		}

		for ( auto elem : input ) {
			grid.emplace_back( elem );
		}
		++numRows;
	}

	void deleteRow() {
		for ( int i = 0; i < numCols; i++ ) {
			grid.pop_back();
		}
		--numRows;
	}

	void clear() {
		grid.clear();
		numCols = 0;
		numRows = 0;
	}

	void reserve( unsigned long long n ) { grid.reserve( n ); }

	T& at( unsigned int row, unsigned int col ) { return this -> operator()( row, col ); }

	const T& at( unsigned int row, unsigned int col ) const { return this -> operator()( row, col ); }

	T& operator() ( unsigned int row, unsigned int col ) { return grid.at( numCols * row + col ); }

	const T& operator() ( unsigned int row, unsigned int col ) const { return grid.at( numCols * row + col ); }

	friend std::ostream& operator<< ( std::ostream& os, const MyTGrid& grid ) {
		os << "Grid: " << grid.numRows << "x" << grid.numCols << "\n";
		int counter{ 0 };
		for ( const auto& value : grid.grid ) {
			os << value;
			if ( counter % grid.numCols == grid.numCols - 1 ) {
				os << "\n";
			}
			++counter;
		}
		os.flush();

		return os;
	}
	
};

#endif // MY_T_GRID_FILE_H