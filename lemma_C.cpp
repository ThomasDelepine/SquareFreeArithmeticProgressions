
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

Code to assert that the bad patterns are 20-constructible. To do so, we explicitely provide the construction

compilation : g++ -O3 -march=native -flto -g -o lemma_C.o lemma_C.cpp
execution   : ./lemma_C.o


*/

void has_motif_notA_delta_B_delta_notC(const std::string s, const char a, const char b, const char c, const int d){
	for(int i = 0; i + 2*d + 2 < s.size(); i++){
		if(s[i] != a && s[i + d + 1] == b && s[i + 2*d + 2] != c){
			std::cout << std::left << std::setw(51) << std::setfill(' ') << s << " contains the pattern (!" << a << ") ⋄^" << std::setw(2) << std::setfill(' ') << d << " (" << b << ") ⋄^" << std::setw(2) << std::setfill(' ') <<  d << " (!" << c << ") at position " << i << std::endl;
			return;
		}
	}
	std::cout << s << " does not contain the pattern" << std::endl;
}


int main(){
	

std::map<char, std::string> h23;
std::map<char, std::string> h24;
std::map<char, std::string> h25;
std::map<char, std::string> h26;

h23['0'] = "01210212021012021201210";
h24['0'] = "012102120210201021201210";
h25['0'] = "0121021202102012021201210";
h26['0'] = "01210212021020121021201210";

h23['1'] = "12021020102120102012021";
h24['1'] = "120210201021012102012021";
h25['1'] = "1202102010210120102012021";
h26['1'] = "12021020102101202102012021";

h23['2'] = "20102101210201210120102";
h24['2'] = "201021012102120210120102";
h25['2'] = "2010210121021201210120102";
h26['2'] = "20102101210212010210120102";


std::cout << "The largest position should be at most 20 (hence (20, h)-constructibility)" << std::endl;
  
// (!a) ⋄^16 (a) ⋄^16 (!a)                                   
has_motif_notA_delta_B_delta_notC(h24['0'] + h26['1'], '0', '0', '0', 16);
has_motif_notA_delta_B_delta_notC(h24['0'] + h26['2'], '0', '0', '0', 16);
has_motif_notA_delta_B_delta_notC(h24['0'] + h26['1'], '1', '1', '1', 16);
has_motif_notA_delta_B_delta_notC(h24['0'] + h26['2'], '1', '1', '1', 16);
has_motif_notA_delta_B_delta_notC(h24['0'] + h26['1'], '2', '2', '2', 16);
has_motif_notA_delta_B_delta_notC(h24['0'] + h26['2'], '2', '2', '2', 16);

// (!a) ⋄^16 (a) ⋄^16 (!(a-1))
has_motif_notA_delta_B_delta_notC(h24['0'] + h26['1'], '0', '0', '2', 16);
has_motif_notA_delta_B_delta_notC(h24['0'] + h26['2'], '0', '0', '2', 16);
has_motif_notA_delta_B_delta_notC(h24['0'] + h26['1'], '1', '1', '0', 16);
has_motif_notA_delta_B_delta_notC(h24['0'] + h26['2'], '1', '1', '0', 16);
has_motif_notA_delta_B_delta_notC(h24['0'] + h26['1'], '2', '2', '1', 16);
has_motif_notA_delta_B_delta_notC(h24['0'] + h26['2'], '2', '2', '1', 16);

// (!a) ⋄^14 (a) ⋄^16 (!(a+1))
has_motif_notA_delta_B_delta_notC(h24['0'] + h26['1'], '0', '0', '1', 14);
has_motif_notA_delta_B_delta_notC(h24['0'] + h26['2'], '0', '0', '1', 14);
has_motif_notA_delta_B_delta_notC(h24['0'] + h26['1'], '1', '1', '2', 14);
has_motif_notA_delta_B_delta_notC(h24['0'] + h26['2'], '1', '1', '2', 14);
has_motif_notA_delta_B_delta_notC(h24['0'] + h26['1'], '2', '2', '0', 14);
has_motif_notA_delta_B_delta_notC(h24['0'] + h26['2'], '2', '2', '0', 14);

// (!a) ⋄^12 (a) ⋄^12 (!a)
has_motif_notA_delta_B_delta_notC(h24['0'] + h26['1'], '0', '0', '0', 12);
has_motif_notA_delta_B_delta_notC(h24['0'] + h26['2'], '0', '0', '0', 12);
has_motif_notA_delta_B_delta_notC(h24['0'] + h26['1'], '1', '1', '1', 12);
has_motif_notA_delta_B_delta_notC(h24['0'] + h26['2'], '1', '1', '1', 12);
has_motif_notA_delta_B_delta_notC(h24['0'] + h26['1'], '2', '2', '2', 12);
has_motif_notA_delta_B_delta_notC(h24['0'] + h26['2'], '2', '2', '2', 12);

// (!a) ⋄^12 (a-1) ⋄^12 (!a)
has_motif_notA_delta_B_delta_notC(h24['0'] + h26['1'], '0', '2', '0', 12);
has_motif_notA_delta_B_delta_notC(h24['0'] + h26['2'], '0', '2', '0', 12);
has_motif_notA_delta_B_delta_notC(h24['0'] + h26['1'], '1', '0', '1', 12);
has_motif_notA_delta_B_delta_notC(h24['0'] + h26['2'], '1', '0', '1', 12);
has_motif_notA_delta_B_delta_notC(h24['0'] + h26['1'], '2', '1', '2', 12);
has_motif_notA_delta_B_delta_notC(h24['0'] + h26['2'], '2', '1', '2', 12);

// (!a) ⋄^12 (a+1) ⋄^12 (!(a - 1))
has_motif_notA_delta_B_delta_notC(h24['0'] + h26['1'], '0', '1', '2', 12);
has_motif_notA_delta_B_delta_notC(h24['0'] + h26['2'], '0', '1', '2', 12);
has_motif_notA_delta_B_delta_notC(h24['0'] + h26['1'], '1', '2', '0', 12);
has_motif_notA_delta_B_delta_notC(h24['0'] + h26['2'], '1', '2', '0', 12);
has_motif_notA_delta_B_delta_notC(h24['0'] + h26['1'], '2', '0', '1', 12);
has_motif_notA_delta_B_delta_notC(h24['0'] + h26['2'], '2', '0', '1', 12);

// (!a) ⋄^10 (a+1) ⋄^10 (!a)
has_motif_notA_delta_B_delta_notC(h24['0'] + h26['1'], '0', '1', '0', 10);
has_motif_notA_delta_B_delta_notC(h24['0'] + h26['2'], '0', '1', '0', 10);
has_motif_notA_delta_B_delta_notC(h24['0'] + h26['1'], '1', '2', '1', 10);
has_motif_notA_delta_B_delta_notC(h24['0'] + h26['2'], '1', '2', '1', 10);
has_motif_notA_delta_B_delta_notC(h24['0'] + h26['1'], '2', '0', '2', 10);
has_motif_notA_delta_B_delta_notC(h24['0'] + h26['2'], '2', '0', '2', 10);

// (!a) ⋄^8 (a-1) ⋄^8 (!a)
has_motif_notA_delta_B_delta_notC(h24['0'] + h26['1'], '0', '2', '0', 8);
has_motif_notA_delta_B_delta_notC(h24['0'] + h26['2'], '0', '2', '0', 8);
has_motif_notA_delta_B_delta_notC(h25['0'] + h26['1'], '1', '0', '1', 8);
has_motif_notA_delta_B_delta_notC(h24['0'] + h26['2'], '1', '0', '1', 8);
has_motif_notA_delta_B_delta_notC(h24['0'] + h26['1'], '2', '1', '2', 8);
has_motif_notA_delta_B_delta_notC(h24['0'] + h26['2'], '2', '1', '2', 8);

// (!a) ⋄^8 (a+1) ⋄^9 (!(a-1))
has_motif_notA_delta_B_delta_notC(h24['0'] + h26['1'], '0', '1', '2', 8);
has_motif_notA_delta_B_delta_notC(h24['0'] + h26['2'], '0', '1', '2', 8);
has_motif_notA_delta_B_delta_notC(h24['0'] + h26['1'], '1', '2', '0', 8);
has_motif_notA_delta_B_delta_notC(h24['0'] + h26['2'], '1', '2', '0', 8);
has_motif_notA_delta_B_delta_notC(h24['0'] + h26['1'], '2', '0', '1', 8);
has_motif_notA_delta_B_delta_notC(h24['0'] + h26['2'], '2', '0', '1', 8);

// (!a) ⋄^6 (a-1) ⋄^6 (!(a+1))
has_motif_notA_delta_B_delta_notC(h24['0'] + h26['1'], '0', '2', '1', 6);
has_motif_notA_delta_B_delta_notC(h24['0'] + h26['2'], '0', '2', '1', 6);
has_motif_notA_delta_B_delta_notC(h24['0'] + h26['1'], '1', '0', '2', 6);
has_motif_notA_delta_B_delta_notC(h24['0'] + h26['2'], '1', '0', '2', 6);
has_motif_notA_delta_B_delta_notC(h24['0'] + h26['1'], '2', '1', '0', 6);
has_motif_notA_delta_B_delta_notC(h24['0'] + h26['2'], '2', '1', '0', 6);

// (!a) ⋄^0 (a-1) ⋄^0 (!a)
has_motif_notA_delta_B_delta_notC(h24['0'] + h26['1'], '0', '2', '0', 0);
has_motif_notA_delta_B_delta_notC(h24['0'] + h26['2'], '0', '2', '0', 0);
has_motif_notA_delta_B_delta_notC(h24['0'] + h26['1'], '1', '0', '1', 0);
has_motif_notA_delta_B_delta_notC(h24['0'] + h26['2'], '1', '0', '1', 0);
has_motif_notA_delta_B_delta_notC(h24['0'] + h26['1'], '2', '1', '2', 0);
has_motif_notA_delta_B_delta_notC(h24['0'] + h26['2'], '2', '1', '2', 0);

// (!a) ⋄^0 (a+1) ⋄^0 (!a)
has_motif_notA_delta_B_delta_notC(h24['0'] + h26['1'], '0', '1', '0', 0);
has_motif_notA_delta_B_delta_notC(h24['0'] + h26['2'], '0', '1', '0', 0);
has_motif_notA_delta_B_delta_notC(h24['0'] + h26['1'], '1', '2', '1', 0);
has_motif_notA_delta_B_delta_notC(h24['0'] + h26['2'], '1', '2', '1', 0);
has_motif_notA_delta_B_delta_notC(h23['0'] + h26['1'], '2', '0', '2', 0);
has_motif_notA_delta_B_delta_notC(h23['0'] + h26['2'], '2', '0', '2', 0);


	return 0;
}