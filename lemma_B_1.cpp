#include <iostream>
#include <iomanip>
#include <string.h>
#include <vector>
#include "pattern.hpp"
/*

Code to assert that the pattern (!a)⋄^d(!b) is (6, h)-recurrent for d in {0..17}

compilation : g++ -O3 -march=native -flto -g -o lemma_B_1.o lemma_B_1.cpp
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
    bool occursAtPosition(const std::string& s, int pos){
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
	std::cout << "The largest distance should be 7 (hence (6,h)-recurrence)" << std::endl;
    auto factors = getFactors(30);
	for(char a : {'0', '1', '2'}){
		for(char b : {'0', '1', '2'}){
			for(int d = 0; d <= 17; d++){
                Pattern_na_d_nb pat(a, b, d);
                int dist = pat.max_distance_before_pattern(factors);
                std::cout << std::left << "For the pattern "<<pat<< ", the largest distance is " << dist << std::endl;
			}
		}
	}
}	

