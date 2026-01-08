#include <iostream>
#include <iomanip>
#include <string.h>
#include <vector>
#include "pattern.hpp"

/*

Code to assert that the pattern P = (!a)⋄^d(b)⋄^d(!c) is 27-recurrent for d in {0..17} and P not in Pbad 

compilation : g++ -O3 -march=native -flto -g -o lemma_B_2.o lemma_B_2.cpp
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

char p(const char a){
	if( a == '2') return '0';
	return a+1;
}
char m(const char a){
	if(a == '0')
		return '2';
	return a-1;
}

bool not_in_Pbad(const char a, const char b, const char c, const int distance){
	if(distance == 16 && a == b && b == c)
		return false;
	if(distance == 16 && a == b && c == m(a))
		return false;
	if(distance == 14 && a == b && c == p(a))
		return false;
	if(distance == 12 && a == b && b == c)
		return false;
	if(distance == 12 && b == m(a) && c == a)
		return false;
	if(distance == 12 && b == p(a) && c == m(a))
		return false;
	if(distance == 10 && b == p(a) && c == a)
		return false;
	if(distance == 8 && b == m(a) && c == a)
		return false;
	if(distance == 8 && b == p(a) && c == m(a))
		return false;
	if(distance == 6 && b == m(a) && c == p(a))
		return false;
	if(distance == 0 && b == m(a) && c == a)
		return false;
	if(distance == 0 && b == p(a) && c == a)
		return false; 
	return true;
}

int main() {
	std::cout << "The largest distance should be 27 (hence (27,h)-recurrence)" << std::endl;
	std::set<std::string> factors = getFactors(70);
	for(char a : {'0', '1', '2'}){
		for(char b : {'0', '1', '2'}){
			for(char c : {'0', '1', '2'}){
				for(int d = 0; d <= 17; d++){
					if(not_in_Pbad(a, b, c, d)){
 						Pattern_na_d_b_d_nc pat(a, b, c, d);
                        int dist = pat.max_distance_before_pattern(factors);
                        std::cout << std::left << "For the pattern "<<pat<< ", the largest distance is " << dist << std::endl;
					}
				}
			}
		}
	}
}	
