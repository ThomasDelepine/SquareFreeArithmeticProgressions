

#include <iostream>
#include <algorithm>
#include <string.h>
#include <stack>
#include <cassert>
#include <time.h>
#include <chrono>
#include <map>
#include <array>
#include <vector>
#include <set>

/*

Code to assert that 
	- the pattern (a)⋄^d(b) is (27, h26)-recurrent for d in {1, 2, 4, 5, 6, 7, 8}, and
	- the pattern ab is (12, h26)-recurrent for a != b

To do so, we generate all possible factors of length 60 of h(t) for every infinite ternary square-free word t
(so in particular, it is enough to generate the factors for every square-free words of length 4)

compilation : g++ -O3 -march=native -flto -g -o lemma_A.o lemma_A.cpp
execution   : ./lemma_A.o

		h26(0) = 01210212021020121021201210
		h26(1) = 12021020102101202102012021
		h26(2) = 20102101210212010210120102
*/

std::vector<std::string> all_squares_of_size_4(){
	std::vector<std::string> res;
	for(const std::string a : {"0", "1", "2"}){
		for(const std::string b : {"0", "1", "2"}){
			for(const std::string c : {"0", "1", "2"}){
				for(const std::string d : {"0", "1", "2"}){
					// abcd is not square-free if and only if a=b or b=c or c=d (square of period 1) or abcd = abab (square of period 2)
					if(a != b && b != c && c != d && (a != c || b != d)){
						res.push_back(a+b+c+d);
					}
				}
			}
		}
	}
	return res;
}

void factors_of_s(const std::string& s, const int size, std::set<std::string>& acc){
	/*
	stores every factor of s of length size in acc
	*/
	for(int i = 0; i <= s.size() - size; i++){
		std::string tmp = "";
		for(int delta = 0; delta < size; delta++){
			tmp = tmp + s[i + delta];
		}
		acc.insert(tmp);
	}
}

std::set<std::string>factors(const int size){
	/*
	Generate all factors of size size of h26(t) for every square-free word t of size 4
	*/
	const std::vector<std::string> h26 = {"01210212021020121021201210", "12021020102101202102012021", "20102101210212010210120102"};
	std::set<std::string> res;
	for(const std::string s : all_squares_of_size_4()){
		factors_of_s(h26[s[0] - '0'] + h26[s[1] - '0'] + h26[s[2] - '0'] + h26[s[3] - '0'], size, res);
	}
	return res;
}

int max_distance_between_patterns(const char a, const char b, int d, const std::set<std::string>& factors){
	/*
	Returns the maximum distance between the two left-most patterns (a)⋄^d(b) amongst every factor in factors

	If one factor has at most 1 such pattern, the function does not terminate (should always terminate)
	*/
	int pos1, pos2, max;
	max = 0;
	for(const std::string s : factors){
		int cpt = 0;
		// we find the first occurence of the pattern
		while(s[cpt] != a || s[cpt + d + 1] != b){
			cpt++;
		}
		pos1 = cpt;
		cpt++;
		// we find the second occurence of the pattern
		while(s[cpt] != a || s[cpt + d + 1] != b){
			cpt++;
		}
		pos2 = cpt;
		max = std::max(max, pos2 - pos1);
	}
	return max;
}

int main() {
	std::cout << "The largest distance should be 28 (hence (27,h)-recurrence)" << std::endl;
	std::set<std::string> factors_set = factors(60);
	for(char a : {'0', '1', '2'}){
		for(char b : {'0', '1', '2'}){
			for(const int d : {1,2,4,5,6,7,8}){
				std::cout << "For the pattern (" << a << ") ⋄^" << d << " (" << b << "), the largest distance is ";
				std::cout << max_distance_between_patterns(a, b, d, factors_set) << std::endl;
			}
		}
	}
	std::cout << "The largest distance should be 13 (hence (12,h)-recurrence)" << std::endl;
	for(char a : {'0', '1', '2'}){
		for(char b : {'0', '1', '2'}){
			if(a != b){
				std::cout << "For the pattern (" << a << ") ⋄^" << 0 << " (" << b << "), the largest distance is ";
				std::cout << max_distance_between_patterns(a, b, 0, factors_set) << std::endl;
			}
		}
	}	
}	
