#include <map>
#include <iostream>
#include <iomanip>
#include <vector>
#include "utility.hpp"

/*

Code that verifies that the pattern (a)⋄^3(b) is (10, h)-constructible and that the patterns (a)⋄^d(b) are (8, h)-constructible for every d in {9, ..., 15}. 

To do so, for every such pattern and for every square-free word w of length 2, we find the best possible guiding sequence.

compilation : g++ -O3 -o lemma_F.o lemma_F.cpp
execution   : ./lemma_F.o


*/

class Pattern_a_d_b: public Pattern{
public:
    char a;
    char b;
    int d;
    Pattern_a_d_b(char a_, char b_, int d_): a(a_), b(b_), d(d_){}
    bool occursAtPosition(const std::string& s, size_t pos){
        if(pos + d + 1 >= s.size())
            return false;
		return (s[pos] == a && s[pos + d + 1] == b);
    }
    friend std::ostream& operator<< (std::ostream& stream, const Pattern_a_d_b& p) {
        stream << "(" << p.a << ") ⋄^" << p.d << " (" << p.b << ")";
        return stream;
    }
};


int main(){
	std::map<int, std::vector<std::string>>h;
	h[23] = {"01210212021012021201210", "12021020102120102012021", "20102101210201210120102"};
	h[24] = {"012102120210201021201210", "120210201021012102012021", "201021012102120210120102"};
	h[25] = {"0121021202102012021201210", "1202102010210120102012021", "2010210121021201210120102"};
	h[26] = {"01210212021020121021201210", "12021020102101202102012021", "20102101210212010210120102"};

    std::cout << "Starting the verification that the patterns (a)⋄^3(b) are (10,h)-constructible." << std::endl;
	std::cout << "The largest position should be at most 10 (hence (10, h)-constructability)" << std::endl;
	int constructibilityBound = 0;
	// For every pattern:
    for(const auto & a : {'0', '1', '2'}){
    for(const auto & b : {'0', '1', '2'}){
            Pattern_a_d_b pat(a, b, 3);
            // for every square-free word of length 2
            for(const auto & w : {"01", "02", "10", "12", "20", "21"}){
                int min = -1; // best constructability bound yet for pat and w
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
                    std::cout << "Error: could not construct pattern " << pat << " with word " << w << "." << std::endl;
                    return 1;
                }
                std::cout << "For the word " << w << ", and the pattern " << pat << ", the best guiding sequence is {" 
                    << mings1 << ", " << mings2 << "}, and the pattern occurs at position " << min << " in" << " " 
                    << h[mings1][w[0] - '0'] + h[mings2][w[1] - '0'] << "." << std::endl;
                if(min > constructibilityBound){
                    constructibilityBound = min;
                }
            }
        } 
    } 
	
	std::cout << "Hence, all the patterns (a)⋄^3(b) are (" << constructibilityBound << ", h)-constructible." << std::endl;
    std::cout << "----------------------------------------" << std::endl;
	std::cout << "Starting the verification that the patterns (a)⋄^d(b) with 9 <= d <= 15 are (8,h)-constructible." << std::endl;
	std::cout << "The largest position should be at most 8 (hence (8, h)-constructability)" << std::endl;
	constructibilityBound = 0;
	// For every pattern:
	for(int i = 9; i <= 15; i++){
        for(const auto & a : {'0', '1', '2'}){
        for(const auto & b : {'0', '1', '2'}){
                Pattern_a_d_b pat(a, b, i);
                // for every square-free word of length 2
                for(const auto & w : {"01", "02", "10", "12", "20", "21"}){
                    int min = -1; // best constructability bound yet for pat and w
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
                        std::cout << "Error: could not construct pattern " << pat << " with word " << w << "." << std::endl;
                        return 1;
                    }
                    std::cout << "For the word " << w << ", and the pattern " << pat << ", the best guiding sequence is {" 
                        << mings1 << ", " << mings2 << "}, and the pattern occurs at position " << min << " in" << " " 
                        << h[mings1][w[0] - '0'] + h[mings2][w[1] - '0'] << "." << std::endl;
                    if(min > constructibilityBound){
                        constructibilityBound = min;
                    }
                }
            } 
        } 
	}
	std::cout << "Hence, all the patterns (a)⋄^d(b) with 9 <= d <= 15 are (" << constructibilityBound << ", h)-constructible." << std::endl;
	return 0;
}