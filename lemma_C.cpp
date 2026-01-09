#include <map>
#include <iostream>
#include <iomanip>
#include <string.h>
#include <vector>
#include "utility.hpp"

/*

Code to assert that the bad patterns are 13-constructible. To do so, we explicitely provide the construction

To do so, for every pattern P in PBad and for every square-free word w of length 2, find find the best possible guiding sequence.

compilation : g++ -O3 -march=native -flto -g -o lemma_C.o lemma_C.cpp
execution   : ./lemma_C.o


*/

class Pattern_na_d_b_d_nc: public Pattern{
public:
    char a;
    char b;
    char c;
    int d;
    Pattern_na_d_b_d_nc(char a_, char b_, char c_, int d_): a(a_), b(b_), c(c_), d(d_){}
    bool occursAtPosition(const std::string& s, int pos){
        if(pos + 2*d + 2 >= s.size())
            return false;
        return (s[pos] != a && s[pos + d + 1] == b && s[pos + 2*d + 2] != c);
    }
    friend std::ostream& operator<< (std::ostream& stream, const Pattern_na_d_b_d_nc& p) {
        stream << "(!" << p.a << ") ⋄^" << p.d << " (" << p.b << ") ⋄^" << p.d << " (!" << p.c << ")";
        return stream;
    }
};


int main(){
	std::map<int, std::vector<std::string>>h;
	h[23] = {"01210212021012021201210", "12021020102120102012021", "20102101210201210120102"};
	h[24] = {"012102120210201021201210", "120210201021012102012021", "201021012102120210120102"};
	h[25] = {"0121021202102012021201210", "1202102010210120102012021", "2010210121021201210120102"};
	h[26] = {"01210212021020121021201210", "12021020102101202102012021", "20102101210212010210120102"};


	std::cout << "The largest position should be at most 13 (hence (13, h)-constructibility)" << std::endl;
	int constructibilityBound = 0;
	// For every pattern in PBad :
	for(const auto & t : PBad){
		Pattern_na_d_b_d_nc pat(std::get<0>(t), std::get<1>(t), std::get<2>(t), std::get<3>(t));
		// for every square-free word of length 2
		for(const auto & w : {"01", "02", "10", "12", "20", "21"}){
			int min = -1; // best constructibility bound yet for pat and w
			int mings1, mings2 = 23; // best guiding sequence yet
			for(const auto gs1 : {23, 24, 25, 26}){
				for(const auto gs2 : {23, 24, 25, 26}){
					int tmp = pat.firstOccurrence(h[gs1][w[0] - '0'] + h[gs2][w[1] - '0']);
					// If gs1, gs2 is a better guiding sequence constructing pat
					if(tmp != -1 && (tmp < min || min == -1)){
						min = tmp;
						mings1 = gs1;
						mings2 = gs2;
					}
				}
			}
			if(min == -1){
				std::cout << "Error: could not construct pattern " << pat << " with word " << w << std::endl;
				return -1;
			}
			std::cout << "For the word " << w << ", and the pattern " << pat << ", the best guiding sequence is {" << mings1 << ", " << mings2 << "}, and the pattern occurs at position " << min << " in" << " " << h[mings1][w[0] - '0'] + h[mings2][w[1] - '0'] << std::endl;
			if(min > constructibilityBound){
				constructibilityBound = min;
			}
		}
	}
	std::cout << "Hence, all patterns in PBad are (" << constructibilityBound << ", h)-constructible." << std::endl;

	return 0;
}