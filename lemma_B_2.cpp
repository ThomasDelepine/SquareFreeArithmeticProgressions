#include <iostream>
#include <iomanip>
#include <string.h>
#include <vector>
#include "utility.hpp"

/*

Code that verifies that the patterns P = (!a)⋄^d(b)⋄^d(!c) are (27, h)-recurrent for d in {0..17} and P not in Pbad 

To do so, we generate all possible factors of length 70 of h(t) for every infinite ternary square-free word t
and we check that each pattern occurs at position at most 27 in every factor

compilation : g++ -O3 -o lemma_B_2.o lemma_B_2.cpp
execution   : ./lemma_B_2.o

		h26(0) = 01210212021020121021201210
		h26(1) = 12021020102101202102012021
		h26(2) = 20102101210212010210120102
*/

class Pattern_na_d_b_d_nc: public Pattern{
public:
    char a;
    char b;
    char c;
    int d;
    Pattern_na_d_b_d_nc(char a_, char b_, char c_, int d_): a(a_), b(b_), c(c_), d(d_){}
    bool occursAtPosition(const std::string& s, size_t pos){
        if(pos + 2*d + 2 >= s.size())
            return false;
        return (s[pos] != a && s[pos + d + 1] == b && s[pos + 2*d + 2] != c);
    }
    friend std::ostream& operator<< (std::ostream& stream, const Pattern_na_d_b_d_nc& p) {
        stream << "(!" << p.a << ") ⋄^" << p.d << " (" << p.b << ") ⋄^" << p.d << " (!" << p.c << ")";
        return stream;
    }
};

int main() {
	std::cout << "Starting the verification that patterns P = (!a)⋄^d(b)⋄^d(!c) are (27,h)-recurrent for d in {0..17} and P not in PBad." << std::endl;
	std::cout << "The largest distance should be 27 (hence (27,h)-recurrence)." << std::endl;
	std::set<std::string> factors = getFactors(70);
	int max = 0;
	for(char a : {'0', '1', '2'}){
		for(char b : {'0', '1', '2'}){
			for(char c : {'0', '1', '2'}){
				for(int d = 0; d <= 17; d++){
					if(!inPBad(t(a, b, c, d))){ // Here, we do not want to test the patterns in PBad
 						Pattern_na_d_b_d_nc pat(a, b, c, d);
                        int dist = pat.max_distance_before_pattern(factors);
						if(dist == -1){ 
							std::cout << "The pattern " << pat << " does not occur." << std::endl;
							return -1;
						}
						else{
							max = std::max(max, dist);
							std::cout << "For the pattern "<<pat<< ", the largest distance is "<<dist<<"."<<std::endl;
						}
					}
				}
			}
		}
	}
	std::cout << "The largest distance was " << max << "." << std::endl;
}	
