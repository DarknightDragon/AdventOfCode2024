#include <iostream>
#include <fstream>
#include <string>
#include <regex>
#include <vector>
#include <cmath> // std::logBASE()

class BinaryCounter {
private:
	std::vector<bool> bin;
	unsigned long long num{ 0 };
	unsigned long long cap{ 0 };

	std::string toStringHelper() {
		std::string res;
		res.reserve( bin.size() );
		for ( auto val = bin.rbegin(); val != bin.rend(); val++ ) {
			res += *val ? "1" : "0";
		}

		return res;
	}

public:
	BinaryCounter() {
		bin.reserve( 64 );
		bin.emplace_back( false );
	}

	BinaryCounter( unsigned long long n ) {
		num = n;
		bin.reserve( 64 );

		switch ( n ) {
		case 0:
			bin.emplace_back( false );
			break;
		case 1:
			bin.emplace_back( true );
			break;
		default:
			while ( n > 0 ) {
				if ( n % 2 == 1 ) {
					bin.emplace_back( true );
				}
				else {
					bin.emplace_back( false );
				}
				n /= 2;
			}
		}
	}

	std::size_t size() const { return bin.size(); }

	void setVal( unsigned long long n ) {
		if ( num == n ) {
			return;
		}

		num = n;
		bin.clear();
		bin.reserve( 64 );
		while ( n > 0 ) {
			if ( n % 2 == 1 ) {
				bin.emplace_back( true );
			}
			else {
				bin.emplace_back( false );
			}
			n /= 2;
		}
	}

	unsigned long long getVal() const { return num; }

	void setCap( unsigned long long n ) { cap = n; }

	unsigned long long getCap() const { return cap; }

	std::string toString() { return toStringHelper(); }

	std::string toStringPadded() { return toStringPadded( std::log2( cap ) + 1 ); }

	std::string toStringPadded( unsigned long long n ) {
		std::string res;
		auto binSize = bin.size();
		res.reserve( n + binSize );
		for ( auto i = n; i > binSize; i-- ) {
			res += "0";
		}

		return res + toStringHelper();
	}

	void reset() {
		num = 0;
		cap = 0;
		bin.clear();
		bin.reserve( 64 );
		bin.emplace_back( false );
	}

	void clear() { reset(); }

	// pre-increment overload
	BinaryCounter& operator++() {
		// if cap is met, do nothing
		if ( cap == 0 || cap > num ) {
			++num;
			for ( auto i = 0; i < bin.size(); i++ ) {
				if ( bin.at( i ) ) {
					bin.at( i ) = false;
					if ( i == bin.size() - 1 ) {
						bin.emplace_back( true );
						break;
					}
				}
				else {
					bin.at( i ) = true;
					break;
				}
			}
		}

		return *this;
	}

	// post-increment overload
	BinaryCounter operator++( int ) {
		BinaryCounter tmp( *this );
		operator++();

		return tmp;
	}
};

bool isValidEquationP1( const std::vector<unsigned long long>& nums );
bool isValidEquationP2( const std::vector<unsigned long long>& nums );

int main() {
	std::ifstream file( "Data.txt" );
	std::regex pattern{ R"((\d+))" };
	const std::sregex_token_iterator endIterator;
	unsigned long long totalCalibrationResultP1{ 0 }, totalCalibrationResultP2{ 0 };
	std::vector<unsigned long long> nums;

	if ( !file.is_open() ) {
		std::cerr << "File error" << std::endl;
		return 1;
	}
	
	for ( std::string line; std::getline( file, line ); ) {
		std::sregex_token_iterator digits { line.begin(), line.end(), pattern };
		nums.reserve( line.length() );

		while ( digits != endIterator ) {
			nums.emplace_back( std::stoull( *digits++ ) );
		}

		if ( isValidEquationP1( nums ) ) {
			totalCalibrationResultP1 += nums.at( 0 );
		}
		if ( isValidEquationP2( nums ) ) {
			totalCalibrationResultP2 += nums.at( 0 );
		}
		nums.clear();
	}

	file.close();

	std::cout << "Total calibration result Part1: " << totalCalibrationResultP1 << std::endl;
	std::cout << "Total calibration result Part2: " << totalCalibrationResultP2 << std::endl;

	return 0;
}

bool isValidEquationP1( const std::vector<unsigned long long>& nums ) {
	if ( nums.empty() || nums.size() == 1 || ( nums.size() == 2 && nums.at( 0 ) != nums.at( 1 ) ) ) {
		return false;
	}

	unsigned long long expectedVal{ nums[0] }, actualValMul{ 1 }, actualValAdd{ 0 };

	for ( auto i = 1; i < nums.size(); i++ ) {
		actualValMul *= nums[i];
		actualValAdd += nums[i];
	}
	if ( expectedVal == actualValMul || expectedVal == actualValAdd ) {
		return true;
	}

	bool isValid{ false };

	// 0 is addition, 1 is multiplication
	BinaryCounter bin{ 1 };
	unsigned long long capSize = std::pow( 2, nums.size() - 2 ) - 1;
	bin.setCap( capSize );
	
	do {
		std::string binString = bin.toStringPadded();
		unsigned long long actualVal{ nums.at( 1 ) };
		for ( auto i = 0; i < binString.length(); i++ ) {
			if ( binString.at( i ) == '0' ) {
				actualVal += nums.at( i + 2 );
			}
			else {
				actualVal *= nums.at( i + 2 );
			}
		}
		if ( expectedVal == actualVal ) {
			isValid = true;
			break;
		}
	} while ( bin++.getVal() < capSize - 1 );

	return isValid;
}

bool isValidEquationP2( const std::vector<unsigned long long>& nums ) {
	if ( nums.empty() || nums.size() == 1 || ( nums.size() == 2 && nums.at( 0 ) != nums.at( 1 ) ) ) {
		return false;
	}

	unsigned long long expectedVal{ nums[0] };

	bool isValid { false };

	// 0 is addition, 1 is multiplication, 2 is concatenation
	unsigned long long maxNumOps = std::pow( 3, nums.size() - 2 );
	std::vector<char> ops; ops.reserve( nums.size() );
	for ( auto i = 0; i < nums.size() - 2; i++ ) {
		ops.emplace_back( '0' );
	}

	for ( auto i = 0; i < maxNumOps; i++ ) {
		unsigned long long actualVal = nums.at( 1 );
		for ( auto j = 0; j < ops.size(); j++ ) {
			switch ( ops.at( j ) ) {
			case '0':
				actualVal += nums.at( j + 2 );
				break;
			case '1':
				actualVal *= nums.at( j + 2 );
				break;
			case '2':
				actualVal *= std::pow( 10, std::floorl( std::log10l( nums.at( j + 2 ) ) ) + 1 );
				actualVal += nums.at( j + 2 );
				break;
			default:
				std::cerr << "Something went very wrong! Ops char is: " + ops.at( j ) << std::endl;
				return false;
			}
		}

		if ( expectedVal == actualVal ) {
			isValid = true;
			break;
		}
		else {
			for ( auto val = ops.rbegin(); val != ops.rend(); val++ ) {
				switch ( *val ) {
				case '0':
					*val = '1';
					break;
				case '1':
					*val = '2';
					break;
				case '2':
					*val = '0';
				}
				if ( *val != '0' ) {
					break;
				}
			}
		}
	}

	return isValid;
}