#include <iostream>
#include <iomanip>
#include <string.h>
#include <vector>
#include <set>

/* Functions to generate all factors */

std::vector<std::string> all_square_free_words_of_size_4(){
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

std::set<std::string> getFactors(const int size_for_factors){
	const std::vector<std::string> h26 = {"01210212021020121021201210", "12021020102101202102012021", "20102101210212010210120102"};
	std::set<std::string> res;
	for(const std::string &s : all_square_free_words_of_size_4()){
		std::string img = h26[s[0] - '0'] + h26[s[1] - '0'] + h26[s[2] - '0'] + h26[s[3] - '0'];	
		for(size_t i = 0; i <= img.size() - size_for_factors; i++){
			res.insert(img.substr(i, size_for_factors));
		}
	}
	return res;
}

/* Class Pattern */

class Pattern{
public:
	virtual bool occursAtPosition(const std::string& s, size_t pos) = 0;
    int firstOccurrence(const std::string& s){
        size_t pos = 0;
        while(pos < s.size() && !occursAtPosition(s, pos)){
            pos++;
        }
        if(pos == s.size())
            return -1;
        else
            return pos;
    }
	int max_distance_before_pattern(const std::set<std::string>& factors){
		/*
		Returns the maximum distance between the two left-most patterns (!a)⋄^d(!b) amongst every factor in factors

		If one factor has at most 1 such pattern, we assert(false)
		*/
		int largest = 0;
		// Loop over all factors that we are interested in
		for(const std::string &s : factors){
			int pos = firstOccurrence(s);
			if(pos == -1){
				return -1;
			}
			if(largest < pos){
				largest = pos;
			}
		}
		return largest;
	}
};

/* Definition of PBad */

std::tuple<char, char, char, int> t(char a, char b, char c, int d){return std::tuple<char, char, char, int>(a, b, c, d);}

std::vector<std::tuple<char, char, char, int>> PBad = {	t('0', '0', '0', 16), t('1', '1', '1', 16), t('2', '2', '2', 16),  // (!a)⋄^16(a)⋄^16(!a)
														t('0', '0', '2', 16), t('1', '1', '0', 16), t('2', '2', '1', 16),  // (!a)⋄^16(a)⋄^16(!(a-1))
														t('0', '0', '1', 14), t('1', '1', '2', 14), t('2', '2', '0', 14),  // (!a)⋄^14(a)⋄^14(!(a+1))

														t('0', '0', '0', 12), t('1', '1', '1', 12), t('2', '2', '2', 12),  // (!a)⋄^12(a)⋄^12(!a)
														t('0', '2', '0', 12), t('1', '0', '1', 12), t('2', '1', '2', 12),  // (!a)⋄^12(a-1)⋄^12(!a)
														t('0', '1', '2', 12), t('1', '2', '0', 12), t('2', '0', '1', 12),  // (!a)⋄^12(a+1)⋄^12(!(a-1))

														t('0', '1', '0', 10), t('1', '2', '1', 10), t('2', '0', '2', 10),  // (!a)⋄^10(a+1)⋄^10(!a)
														t('0', '2', '0', 8), t('1', '0', '1', 8), t('2', '1', '2', 8),  // (!a)⋄^8(a-1)⋄^8(!a)
														t('0', '1', '2', 8), t('1', '2', '0', 8), t('2', '0', '1', 8),  // (!a)⋄^8(a+1)⋄^8(!(a-1))

														t('0', '2', '1', 6), t('1', '0', '2', 6), t('2', '1', '0', 6),  // (!a)⋄^6(a-1)⋄^6(!(a+1))
														t('0', '2', '0', 0), t('1', '0', '1', 0), t('2', '1', '2', 0),  // (!a)⋄^0(a-1)⋄^0(!a)
														t('0', '1', '0', 0), t('1', '2', '1', 0), t('2', '0', '2', 0),  // (!a)⋄^0(a+1)⋄^0(!a)
													  };

bool inPBad(const std::tuple<char, char, char, int>& t){
	for(const auto & e : PBad){
		if(e == t){
			return true;
		}
	}
	return false;
}
