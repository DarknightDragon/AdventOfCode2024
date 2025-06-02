#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include <cstddef> // std::size_t

std::vector<int> createDiskFromMap( const std::string& diskMap );
std::vector<int> compressBlocks( const std::vector<int>& disk );
std::vector<int> compressFiles( const std::vector<int>& disk );
unsigned long long calculateChecksum( const std::vector<int>& disk );

int main() {
	std::ifstream file( "Data.txt" );

	if ( !file.is_open() ) {
		std::cerr << "File error" << std::endl;
		return 1;
	}

	std::string input;
	std::getline( file, input );
	file.close();

	std::vector<int> disk = createDiskFromMap( input );
	std::vector<int> p1Ans = compressBlocks( disk );
	std::vector<int> p2Ans = compressFiles( disk );

	std::cout << "Part 1 checksum: " << calculateChecksum( p1Ans ) << std::endl;
	std::cout << "Part 2 checksum: " << calculateChecksum( p2Ans ) << std::endl;

	return 0;
}

std::vector<int> createDiskFromMap( const std::string& diskMap ) {
	std::vector<int> ans;
	ans.reserve( diskMap.length() * 9 );

	for ( std::size_t i = 0; i < diskMap.length(); i++ ) {
		ans.insert( ans.end(), diskMap[i] - '0', i % 2 == 0 ? i / 2 : -1 );
	}

	return ans;
}

std::vector<int> compressBlocks( const std::vector<int>& disk ) {
	if ( disk.empty() ) {
		return std::vector<int>();
	}

	std::vector<int> ans{ disk };

	auto leftItr = ans.begin(), rightItr = ans.end();
	--rightItr;
	for ( std::size_t i = 0; leftItr != rightItr && i < ans.size() / 2 + 1; i++ ) {
		while ( leftItr != rightItr && *leftItr != -1 ) {
			++leftItr;
		}
		while ( leftItr != rightItr && *rightItr == -1 ) {
			--rightItr;
		}
		if ( leftItr != rightItr ) {
			*leftItr = *rightItr;
			*rightItr = -1;
		}
	}

	return ans;
}

std::vector<int> compressFiles( const std::vector<int>& disk ) {
	if ( disk.empty() ) {
		return std::vector<int>();
	}

	std::vector<int> ans{ disk };

	auto rightItr = ans.rbegin(), endItr = ans.rend();
	std::size_t rightIdx = ans.size() - 1;
	while ( rightItr != endItr ) {
		while ( *rightItr == -1 ) {
			++rightItr;
			--rightIdx;
		}
		if ( rightItr == endItr ) {
			break;
		}

		int fileId{ *rightItr };
		unsigned long long fileSize{ 0ull };
		unsigned long long freeSize{ 0ull };
		while ( rightItr != endItr && *rightItr == fileId ) {
			++fileSize;
			++rightItr;
			if ( rightIdx != 0 ) {
				--rightIdx;
			}
		}

		for ( std::size_t i = 0; i <= rightIdx; i++ ) {
			if ( ans[i] == -1 ) {
				if ( ++freeSize == fileSize ) {
					for ( std::size_t j = 0; j < fileSize; j++ ) {
						ans[i - fileSize + j + 1] = fileId;
						ans[rightIdx + fileSize - j] = -1;
					}
					freeSize = 0ull;
					break;
				}
			}
			else if ( freeSize != 0 ) {
				freeSize = 0ull;
			}
		}
	}

	return ans;
}

unsigned long long calculateChecksum( const std::vector<int>& disk ) {
	unsigned long long checksum{ 0 };
	for ( std::size_t i = 0; i < disk.size(); i++ ) {
		if ( disk[i] != -1 ) {
			checksum += i * disk[i];
		}
	}

	return checksum;
}
