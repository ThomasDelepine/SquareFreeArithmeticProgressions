
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

Code to assert that the bad patterns are 20-constructible

compilation : g++ -O3 -march=native -flto -g -o exec file.cpp
execution   : ./exec


*/

void has_motif_notA_delta_B_delta_notC(const std::string s, const int a, const int b, const int c, const int distance){
	for(int i = 0; i < s.size()-2*distance; i++){
		if(s[i] != a && s[i + distance] == b && s[i + 2*distance] != c){
			std::cout << i << std::endl;
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




// !a <-17-> a <-17-> !a                                      c <> b <> c
has_motif_notA_delta_B_delta_notC(h24['0'] + h26['1'], '0', '0', '0', 17);
has_motif_notA_delta_B_delta_notC(h24['0'] + h26['2'], '0', '0', '0', 17);
has_motif_notA_delta_B_delta_notC(h24['0'] + h26['1'], '1', '1', '1', 17);
has_motif_notA_delta_B_delta_notC(h24['0'] + h26['2'], '1', '1', '1', 17);
has_motif_notA_delta_B_delta_notC(h24['0'] + h26['1'], '2', '2', '2', 17);
has_motif_notA_delta_B_delta_notC(h24['0'] + h26['2'], '2', '2', '2', 17);

// !a <-17-> a <-17-> !(a-1)
has_motif_notA_delta_B_delta_notC(h24['0'] + h26['1'], '0', '0', '2', 17);
has_motif_notA_delta_B_delta_notC(h24['0'] + h26['2'], '0', '0', '2', 17);
has_motif_notA_delta_B_delta_notC(h24['0'] + h26['1'], '1', '1', '0', 17);
has_motif_notA_delta_B_delta_notC(h24['0'] + h26['2'], '1', '1', '0', 17);
has_motif_notA_delta_B_delta_notC(h24['0'] + h26['1'], '2', '2', '1', 17);
has_motif_notA_delta_B_delta_notC(h24['0'] + h26['2'], '2', '2', '1', 17);

// !a <-15-> a <-15-> !(a+1)
has_motif_notA_delta_B_delta_notC(h24['0'] + h26['1'], '0', '0', '1', 15);
has_motif_notA_delta_B_delta_notC(h24['0'] + h26['2'], '0', '0', '1', 15);
has_motif_notA_delta_B_delta_notC(h24['0'] + h26['1'], '1', '1', '2', 15);
has_motif_notA_delta_B_delta_notC(h24['0'] + h26['2'], '1', '1', '2', 15);
has_motif_notA_delta_B_delta_notC(h24['0'] + h26['1'], '2', '2', '0', 15);
has_motif_notA_delta_B_delta_notC(h24['0'] + h26['2'], '2', '2', '0', 15);

// !a <-13-> a <-13-> !a
has_motif_notA_delta_B_delta_notC(h24['0'] + h26['1'], '0', '0', '0', 13);
has_motif_notA_delta_B_delta_notC(h24['0'] + h26['2'], '0', '0', '0', 13);
has_motif_notA_delta_B_delta_notC(h24['0'] + h26['1'], '1', '1', '1', 13);
has_motif_notA_delta_B_delta_notC(h24['0'] + h26['2'], '1', '1', '1', 13);
has_motif_notA_delta_B_delta_notC(h24['0'] + h26['1'], '2', '2', '2', 13);
has_motif_notA_delta_B_delta_notC(h24['0'] + h26['2'], '2', '2', '2', 13);

// !a <-13-> a-1 <-13-> !a
has_motif_notA_delta_B_delta_notC(h24['0'] + h26['1'], '0', '2', '0', 13);
has_motif_notA_delta_B_delta_notC(h24['0'] + h26['2'], '0', '2', '0', 13);
has_motif_notA_delta_B_delta_notC(h24['0'] + h26['1'], '1', '0', '1', 13);
has_motif_notA_delta_B_delta_notC(h24['0'] + h26['2'], '1', '0', '1', 13);
has_motif_notA_delta_B_delta_notC(h24['0'] + h26['1'], '2', '1', '2', 13);
has_motif_notA_delta_B_delta_notC(h24['0'] + h26['2'], '2', '1', '2', 13);

// !a <-13-> a+1 <-13-> !(a-1)
has_motif_notA_delta_B_delta_notC(h24['0'] + h26['1'], '0', '1', '2', 13);
has_motif_notA_delta_B_delta_notC(h24['0'] + h26['2'], '0', '1', '2', 13);
has_motif_notA_delta_B_delta_notC(h24['0'] + h26['1'], '1', '2', '0', 13);
has_motif_notA_delta_B_delta_notC(h24['0'] + h26['2'], '1', '2', '0', 13);
has_motif_notA_delta_B_delta_notC(h24['0'] + h26['1'], '2', '0', '1', 13);
has_motif_notA_delta_B_delta_notC(h24['0'] + h26['2'], '2', '0', '1', 13);

// !a <-11-> a+1<-11-> !a
has_motif_notA_delta_B_delta_notC(h24['0'] + h26['1'], '0', '1', '0', 11);
has_motif_notA_delta_B_delta_notC(h24['0'] + h26['2'], '0', '1', '0', 11);
has_motif_notA_delta_B_delta_notC(h24['0'] + h26['1'], '1', '2', '1', 11);
has_motif_notA_delta_B_delta_notC(h24['0'] + h26['2'], '1', '2', '1', 11);
has_motif_notA_delta_B_delta_notC(h24['0'] + h26['1'], '2', '0', '2', 11);
has_motif_notA_delta_B_delta_notC(h24['0'] + h26['2'], '2', '0', '2', 11);

// !a <-9-> a-1 <-9-> !a
has_motif_notA_delta_B_delta_notC(h24['0'] + h26['1'], '0', '2', '0', 9);
has_motif_notA_delta_B_delta_notC(h24['0'] + h26['2'], '0', '2', '0', 9);
has_motif_notA_delta_B_delta_notC(h25['0'] + h26['1'], '1', '0', '1', 9);
has_motif_notA_delta_B_delta_notC(h24['0'] + h26['2'], '1', '0', '1', 9);
has_motif_notA_delta_B_delta_notC(h24['0'] + h26['1'], '2', '1', '2', 9);
has_motif_notA_delta_B_delta_notC(h24['0'] + h26['2'], '2', '1', '2', 9);

// !a <-9-> a+1 <-9-> !(a-1)
has_motif_notA_delta_B_delta_notC(h24['0'] + h26['1'], '0', '1', '2', 9);
has_motif_notA_delta_B_delta_notC(h24['0'] + h26['2'], '0', '1', '2', 9);
has_motif_notA_delta_B_delta_notC(h24['0'] + h26['1'], '1', '2', '0', 9);
has_motif_notA_delta_B_delta_notC(h24['0'] + h26['2'], '1', '2', '0', 9);
has_motif_notA_delta_B_delta_notC(h24['0'] + h26['1'], '2', '0', '1', 9);
has_motif_notA_delta_B_delta_notC(h24['0'] + h26['2'], '2', '0', '1', 9);

// !a <-7-> a-1 <-7-> !(a+1)
has_motif_notA_delta_B_delta_notC(h24['0'] + h26['1'], '0', '2', '1', 7);
has_motif_notA_delta_B_delta_notC(h24['0'] + h26['2'], '0', '2', '1', 7);
has_motif_notA_delta_B_delta_notC(h24['0'] + h26['1'], '1', '0', '2', 7);
has_motif_notA_delta_B_delta_notC(h24['0'] + h26['2'], '1', '0', '2', 7);
has_motif_notA_delta_B_delta_notC(h24['0'] + h26['1'], '2', '1', '0', 7);
has_motif_notA_delta_B_delta_notC(h24['0'] + h26['2'], '2', '1', '0', 7);

// !a <-1-> a-1 <-1-> !a
has_motif_notA_delta_B_delta_notC(h24['0'] + h26['1'], '0', '2', '0', 1);
has_motif_notA_delta_B_delta_notC(h24['0'] + h26['2'], '0', '2', '0', 1);
has_motif_notA_delta_B_delta_notC(h24['0'] + h26['1'], '1', '0', '1', 1);
has_motif_notA_delta_B_delta_notC(h24['0'] + h26['2'], '1', '0', '1', 1);
has_motif_notA_delta_B_delta_notC(h24['0'] + h26['1'], '2', '1', '2', 1);
has_motif_notA_delta_B_delta_notC(h24['0'] + h26['2'], '2', '1', '2', 1);

// !a <-1-> a+1 <-1-> !a
has_motif_notA_delta_B_delta_notC(h24['0'] + h26['1'], '0', '1', '0', 1);
has_motif_notA_delta_B_delta_notC(h24['0'] + h26['2'], '0', '1', '0', 1);
has_motif_notA_delta_B_delta_notC(h24['0'] + h26['1'], '1', '2', '1', 1);
has_motif_notA_delta_B_delta_notC(h24['0'] + h26['2'], '1', '2', '1', 1);
has_motif_notA_delta_B_delta_notC(h23['0'] + h26['1'], '2', '0', '2', 1);
has_motif_notA_delta_B_delta_notC(h23['0'] + h26['2'], '2', '0', '2', 1);


	return 0;
}