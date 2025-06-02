#include <iostream>
#include <fstream>
#include <string>
#include <regex>
#include <vector>
#include <cmath>

bool isValidEquationTest( const std::vector<unsigned long long>& nums );

int main1() {
	std::ifstream file( "Data.txt" );
	std::regex pattern { R"((\d+))" };
	const std::sregex_token_iterator endIterator;
	unsigned long long totalCalibrationResult{ 0 };
	std::vector<unsigned long long> nums;

	for ( std::string line; std::getline( file, line ); ) {
		std::sregex_token_iterator digits{ line.begin(), line.end(), pattern };
		nums.reserve( line.length() );

		while ( digits != endIterator ) {
			nums.emplace_back( std::stoull( *digits++ ) );
		}

		//for ( auto i : nums ) { std::cout << i << " "; }std::cout << std::endl;
		if ( isValidEquationTest( nums ) ) {
			totalCalibrationResult += nums.at( 0 );
			//for ( auto i : nums ) { std::cout << i << " "; }std::cout << std::endl;
		}
		nums.clear();
	}

	std::cout << "Total calibration result: " << totalCalibrationResult << std::endl;

	return 0;
}

bool isValidEquationTest( const std::vector<unsigned long long>& nums ) {
	if ( nums.empty() ) {
		return true;
	}
	else if ( nums.size() == 1 || ( nums.size() == 2 && nums.at( 0 ) != nums.at( 1 ) ) ) {
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

	bool isValid { false };

	// 0 is addition, 1 is multiplication
	unsigned long long maxNumOps = std::pow( 2, nums.size() - 2 );
	std::vector<bool> ops; ops.reserve( nums.size() );
	for ( auto i = 0; i < nums.size() - 2; i++ ) { ops.emplace_back( false ); } ops.back() = true;

	for ( auto i = 1; i < maxNumOps; i++ ) {
		unsigned long long actualVal = nums.at( 1 );
		for ( auto j = 0; j < ops.size(); j++ ) {
			if ( ops.at( j ) ) {
				actualVal *= nums.at( j + 2 );
			}
			else {
				actualVal += nums.at( j + 2 );
			}
		}

        if ( expectedVal == actualVal ) {
            isValid = true;
            break;
        }
        else {
            for ( auto val = ops.rbegin(); val != ops.rend(); val++ ) {
                if ( *val ) {
                    *val = false;
                }
                else {
                    *val = true;
                    break;
                }
            }
        }
	}

	return isValid;
}