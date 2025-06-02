#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm> // std::sort
#include <cmath> // std::abs

int main() {
	std::ifstream file( "Lists.txt" );
	std::vector<int> list1;
	std::vector<int> list2;
	int totalDistance = 0;
	long similarityScore = 0;
	list1.reserve( 50 );
	list2.reserve( 50 );

	if ( file.is_open() ) {
		for ( int l1 = 0, l2 = 0; file >> l1 >> l2; ) {
			list1.push_back( l1 );
			list2.push_back( l2 );
		}
		file.close();
	}

	std::sort( list1.begin(), list1.end() );
	std::sort( list2.begin(), list2.end() );
	
	for ( int i = 0; i < list1.size(); i++ ) {
		totalDistance += std::abs( list1[i] - list2[i] );
		
		//part two
		int count = 0;
		for ( int j = 0; j < list2.size(); j++ ) {
			if ( list1[i] == list2[j] ) {
				count++;
			}
		}

		similarityScore += list1[i] * count;
	}
	std::cout << "Total Distance: " << totalDistance << std::endl;
	std::cout << "Similarity Score: " << similarityScore << std::endl;
	
	return 0;
}