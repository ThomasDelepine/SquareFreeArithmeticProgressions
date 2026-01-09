#include <iostream>
#include <iomanip>
#include <string.h>
#include <vector>
#include "utility.hpp"
/*

Code to assert that 
	- the pattern (a)⋄^d(b) is (27, h26)-recurrent for d in {1, 2, 4, 5, 6, 7, 8}, and
	- the pattern ab is (12, h26)-recurrent for a != b

To do so, we generate all possible factors of length 40 of h(t) for every infinite ternary square-free word t
and we check that each pattern occurs at position at most 27 (or 12) in every factor

compilation : g++ -O3 -march=native -flto -g -o lemma_A.o lemma_A.cpp
execution   : ./lemma_A.o

		h26(0) = 01210212021020121021201210
		h26(1) = 12021020102101202102012021
		h26(2) = 20102101210212010210120102
*/

class Pattern_a_d_b: public Pattern{
public:
    char a;
    char b;
    int d;
    Pattern_a_d_b(char a_, char b_, int d_): a(a_), b(b_), d(d_){}
    bool occursAtPosition(const std::string& s, int pos){
        if(pos + d + 1 >= s.size())
            return false;
		return (s[pos] == a && s[pos + d + 1] == b);
    }
    friend std::ostream& operator<< (std::ostream& stream, const Pattern_a_d_b& p) {
        stream << "(" << p.a << ") ⋄^" << p.d << " (" << p.b << ")";
        return stream;
    }
};

int main() {
	std::cout << "The largest distance should be 27 (hence (27,h)-recurrence)" << std::endl;
	std::set<std::string> factors = getFactors(40);
	int max = 0;
	for(char a : {'0', '1', '2'}){
		for(char b : {'0', '1', '2'}){
			for(const int d : {1,2,4,5,6,7,8}){
                Pattern_a_d_b pat(a, b, d);
				int dist = pat.max_distance_before_pattern(factors);
				if(dist == -1) std::cout << "The pattern " << pat << " does not occur" << std::endl;
				else{
					max = std::max(max, dist);
					 std::cout << "For the pattern "<<pat<< ", the largest distance is "<<dist<<std::endl;
				}
			}
		}
	}
	std::cout << "the largest distance was " << max << std::endl << std::endl;
	std::cout << "The largest distance should be 12 (hence (12,h)-recurrence)" << std::endl;
	max = 0;
	for(char a : {'0', '1', '2'}){
		for(char b : {'0', '1', '2'}){
			if(a != b){
                Pattern_a_d_b pat(a, b, 0);
				int dist = pat.max_distance_before_pattern(factors);
				if(dist == -1) std::cout << "The pattern " << pat << " does not occur" << std::endl;
				else{
					max = std::max(max, dist);
					std::cout << "For the pattern "<<pat<< ", the largest distance is "<<dist<<std::endl;
				}
			}
		}
	}
	std::cout << "the largest distance was " << max << std::endl;	
}	
