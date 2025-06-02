//#include <fstream>
//#include <iostream>
//#include <string>
//#include <vector>
//#include <cstddef> // std::size_t
//
//std::vector<std::string> createDiskFromMap( const std::string& diskMap );
//std::vector<std::string> compressBlocks( const std::vector<std::string>& disk );
//std::vector<std::string> compressFiles( const std::vector<std::string>& disk );
//unsigned long long calculateChecksum( const std::vector<std::string>& disk );
//
//int main() {
//	std::ifstream file( "Data.txt" );
//
//	if ( !file.is_open() ) {
//		std::cerr << "File error" << std::endl;
//		return 1;
//	}
//
//	std::string input;
//	std::getline( file, input );
//	std::vector<std::string> disk = createDiskFromMap( input );
//	std::vector<std::string> p1Ans = compressBlocks( disk );
//	std::vector<std::string> p2Ans = compressFiles( disk );
//	
//	std::cout << "Part 1 checksum: " << calculateChecksum( p1Ans ) << std::endl;
//	std::cout << "Part 2 checksum: " << calculateChecksum( p2Ans ) << std::endl;
//
//	return 0;
//}
//
//std::vector<std::string> createDiskFromMap( const std::string& diskMap ) {
//	std::vector<std::string> ans;
//	ans.reserve( diskMap.length() * 9 );
//
//	for ( std::size_t i = 0; i < diskMap.length(); i++ ) {
//		ans.insert( ans.end(), diskMap[i] - '0', i % 2 == 0 ? std::to_string( i / 2 ) : ".");
//	}
//
//	return ans;
//}
//
//std::vector<std::string> compressBlocks( const std::vector<std::string>& disk ) {
//	if ( disk.empty() ) {
//		return std::vector<std::string>();
//	}
//
//	std::vector<std::string> ans{ disk };
//
//	auto leftItr = ans.begin(), rightItr = ans.end();
//	--rightItr;
//	for ( std::size_t i = 0; leftItr != rightItr && i < ans.size() / 2 + 1; i++ ) {
//		while ( leftItr != rightItr && *leftItr != "." ) {
//			++leftItr;
//		}
//		while ( leftItr != rightItr && *rightItr == "." ) {
//			--rightItr;
//		}
//		if ( leftItr != rightItr ) {
//			*leftItr = *rightItr;
//			*rightItr = ".";
//		}
//	}
//
//	return ans;
//}
//
//std::vector<std::string> compressFiles( const std::vector<std::string>& disk ) {
//	if ( disk.empty() ) {
//		return std::vector<std::string>();
//	}
//
//	std::vector<std::string> ans{ disk };
//
//	auto rightItr = ans.rbegin(), endItr = ans.rend();
//	std::size_t rightIdx = ans.size() - 1;
//	while ( rightItr != endItr ) {
//		while ( *rightItr == "." ) {
//			++rightItr;
//			--rightIdx;
//		}
//		if ( rightItr == endItr ) {
//			break;
//		}
//
//		std::string fileId{ *rightItr };
//		unsigned long long fileSize{ 0ull };
//		unsigned long long freeSize{ 0ull };
//		while ( rightItr != endItr && *rightItr == fileId ) {
//			++fileSize;
//			++rightItr;
//			if ( rightIdx != 0 ) {
//				--rightIdx;
//			}
//		}
//
//		for ( std::size_t i = 0; i <= rightIdx; i++ ) {
//			if ( ans[i] == "." ) {
//				if ( ++freeSize == fileSize ) {
//					for ( std::size_t j = 0; j < fileSize; j++ ) {
//						ans[i - fileSize + j + 1] = fileId;
//						ans[rightIdx + fileSize - j] = ".";
//					}
//					freeSize = 0ull;
//					break;
//				}
//			}
//			else if ( freeSize != 0 ) {
//				freeSize = 0ull;
//			}
//		}
//	}
//
//	return ans;
//}
//
//unsigned long long calculateChecksum( const std::vector<std::string>& disk ) {
//	unsigned long long checksum{ 0 };
//	for ( std::size_t i = 0; i < disk.size(); i++ ) {
//		if ( disk[i] != "." ) {
//			checksum += i * std::stoull( disk[i] );
//		}
//	}
//
//	return checksum;
//}
