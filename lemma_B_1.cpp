

#include <iostream>
#include <iomanip>
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

Code to assert that the pattern (!a)⋄^d(!b) is (6, h)-recurrent for d in {0..17}

compilation : g++ -O3 -march=native -flto -g -o lemma_B_1.o lemma_B_1.cpp
execution   : ./lemma_B_1.o

		h26(0) = 01210212021020121021201210
		h26(1) = 12021020102101202102012021
		h26(2) = 20102101210212010210120102
*/

bool checkSquareFreeAtEnd(std::string s, int l){
    /*
    Assume that s = uv with u square free and |v| = l, return true iff s is square free
    */
    if(s.size() <= l)
    	return true;
    for(int period = 1; period <= s.size()/2; period++){
        for(int end = 0; end < std::min(2*period - 1, l); end++){
            int end_pos = s.size() - l + end;
            bool flag = true;
            for(int i = 0; i < period; i++ ){             
                if(s[end_pos - i] != s[end_pos - i - period]){
                    flag = false;
                    break;
                }
            }
            if(flag){
                return false;
            }
        }
    }
    return true;
}




std::pair<bool, int> has_two_patterns(const std::string& s, const char a, const char b, const int d){
	/*
	returns (false, -1) if s does not have the pattern (!a)⋄^distance(!b)
	returns (true, -1)  if s has exactly one occurence of the pattern (!a)⋄^d(!b)
	returns (true, dist)   if s has at least two occurences of the pattern and dist is the distance between the two leftmost patterns (!a)⋄^d(!b)
	*/

	// First, we find a first pattern
	bool has_one_pattern = false;
	int pos_first_pattern;
	for(int i = 0; i < s.size() - d; i++){
		if(s[i] != a && s[i + d + 1] != b){
			has_one_pattern =  true;
			pos_first_pattern = i;
			break;
		}
	}

	// If there is a pattern strating at pos_first_pattern :
	if(has_one_pattern){
		for(int i = pos_first_pattern + 1; i < s.size() - d; i++){
			// If we find a second pattern
			if(s[i] != a && s[i + d + 1] != b){
				return std::pair<bool, int>(true, i - pos_first_pattern);
			}
		}
		// Otherwise there is no second pattern in s
		return std::pair<bool, int>(true, -1);
	}

	// Otherwise there is no pattern in s :
	return std::pair<bool, int>(false, -1);
}


void max_distance_between_patterns(const char a, const char b, int d, std::set<std::string> factors){
	/*
	Returns the maximum distance between the two left-most patterns (!a)⋄^d(!b) amongst every factor in factors

	If one factor has at most 1 such pattern, we assert(false)
	*/
	int largest = 0;
	std::string largest_str = "";
	// Loop over all factors that we are interested in
	for(std::string s : factors){
		std::pair<bool, int> tmp = has_two_patterns(s, a, b, d);
		if(!tmp.first || tmp.second == -1){
			assert(false); 
		}
		else{
			if(largest < tmp.second){
				largest = tmp.second;
				largest_str = s;
			}
		}
	}
	std::cout << std::left << "For the pattern (!" << a << ") ⋄^" << std::setw(2) << std::setfill(' ') <<  d << " (!" << b << "), the largest distance is " << std::to_string(largest) << std::endl;
}

void all_factors(const std::string& s, const int size, std::set<std::string>& acc){
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

std::set<std::string> images_of_h26(const int size_for_factors, const int number_of_images){
	/*

	*/
	// The images of h26 						h26(0)							h26(1) 						h26(2)
	const std::vector<std::string> h26 = {"01210212021020121021201210", "12021020102101202102012021", "20102101210212010210120102"};
	// The set of factors
	std::set<std::string> res;
	// Initialization of the data structure for the search
	std::string s = "";				// the current string
	std::stack<std::string> stack;	// the stack to emulate a DFS
	for(std::string image : h26)
		stack.push(image);
	stack.push(h26[2]);
	stack.push(h26[1]);
	stack.push(h26[0]);

	// while the current word can be extended
	while(!stack.empty()){
		// Depile case
		if(stack.top() == "p"){
			stack.pop();
			for(int i = 0; i < 26; i++){
				s.pop_back();
			}
		}
		// Extension case
		else{
			s = s + stack.top();
			stack.pop();
			// If adding 26 new letters did not create any square :
			if(checkSquareFreeAtEnd(s, 26)){
				// if we've reached the aimed size :
				if(s.size() >= 26*number_of_images){
					// we store the factors
					all_factors(s, size_for_factors, res);
					// then we backtrack
					for(int i = 0; i < 26; i++){
						s.pop_back();
					}
				}
				// Otherwise, we continue to extend the current word
				else{
					stack.push("p");
					stack.push(h26[2]);
					stack.push(h26[1]);
					stack.push(h26[0]);
				}
			}
			// If we created a square, then we backtrack (and pop_back 26 times)
			else{
				for(int i = 0; i < 26; i++){
					s.pop_back();
				}
			}
		}
	}
	return res;
}

int main() {
	std::cout << "The largest distance should be 7 (hence (6,h)-recurrence)" << std::endl;
	for(char a : {'0', '1', '2'}){
		for(char b : {'0', '1', '2'}){
			for(int d = 0; d <= 17; d++){
				max_distance_between_patterns(a, b, d, images_of_h26(200, 9));
			}
		}
	}
}	
