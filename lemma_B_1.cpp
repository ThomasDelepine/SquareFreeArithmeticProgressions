#include <iostream>
#include <iomanip>
#include <vector>
#include "utility.hpp"
/*

Code that verifies that the patterns (!a)⋄^d(!b) are (6, h)-recurrent for d in {0..17}

To do so, we generate all possible factors of length 30 of h(t) for every infinite ternary square-free word t
and we check that each pattern occurs at position at most 6 in every factor

compilation : g++ -O3 -o lemma_B_1.o lemma_B_1.cpp
execution   : ./lemma_B_1.o

		h26(0) = 01210212021020121021201210
		h26(1) = 12021020102101202102012021
		h26(2) = 20102101210212010210120102
*/

class Pattern_na_d_nb: public Pattern{
public:
    char a;
    char b;
    int d;
    Pattern_na_d_nb(char a_, char b_, int d_): a(a_), b(b_), d(d_){}
    bool occursAtPosition(const std::string& s, size_t pos){
        if(pos + d + 1 >= s.size())
            return false;
        return (s[pos] != a && s[pos + d + 1] != b);
    }
    friend std::ostream& operator<< (std::ostream& stream, const Pattern_na_d_nb& p) {
        stream << "(!" << p.a << ") ⋄^" << p.d << " (!" << p.b << ")";
        return stream;
    }
};


int main() {
    std::cout << "Starting the verification that patterns (!a)⋄^d(!b) are (6,h)-recurrent for d in {0..17}." << std::endl;
	std::cout << "The largest distance should be 6 (hence (6,h)-recurrence)." << std::endl;
    auto factors = getFactors(30);
    int max = 0;
	for(char a : {'0', '1', '2'}){
		for(char b : {'0', '1', '2'}){
			for(int d = 0; d <= 17; d++){
                Pattern_na_d_nb pat(a, b, d);
                int dist = pat.max_distance_before_pattern(factors);
                if(dist == -1){ 
                    std::cout << "The pattern " << pat << " does not occur." << std::endl;
                    return 1;
                }
				else{
					max = std::max(max, dist);
					std::cout << "For the pattern "<<pat<< ", the largest distance is "<<dist<<"."<<std::endl;
				}
			}
		}
	}
    std::cout << "The largest distance was " << max << "." << std::endl;
    return 0;
}	

