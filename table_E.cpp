#include <iostream>
#include <fstream>
#include <iomanip>
#include <algorithm>
#include "utility.hpp"


/*

Code that verifies that there exists a morphism that is square-free and whose subsequences congruent to alpha modulo p are also square-free

To do so, we check that the solutions stored in table_E_morphisms.txt are valid

compilation : g++ -O3 -o table_E.o table_E.cpp
execution   : ./table_E.o

*/


std::string pi3(const std::string& s){
    std::string res = "";
    for(const char e : s){
        if(e == '0'){
            res = res + '1';
        }
        else if(e == '1'){
            res = res + '2';
        }
        else{
            res = res + '0';
        }
    }
    return res;
}
    

int main(int argc, char* argv[]){
    if(argc != 2){
        std::cout << "Usage 1 : ./table_E.cpp <p> (with p in {3,4,5,7,8,9,10,11,12,14,15,16,20,21,22})" << std::endl;
        return 1;
    }
    int p = std::stoi(argv[1]);

    // We read the morphism from table_E_morphisms.txt
    std::ifstream file("table_E_morphisms.txt");
    std::string line, h0, h1, h2, h0p, h1p, h2p;
    int pr=0, alpha;
    for (int lineNum = 1; std::getline(file, line); ++lineNum) {
            std::istringstream iss(line);
            iss >> pr >> h0 >> alpha;
            if(iss.fail()) continue;
            if(pr == p) break;
    }
    if(pr != p){
        std::cout << "Usage 2 : ./table_E.cpp <p> (with p in {3,4,5,7,8,9,10,11,12,14,15,16,20,21,22})" << std::endl;
        return 1;
    }

    h1 = pi3(h0);
    h2 = pi3(h1);
    h0p = subsequence(h0, p, alpha);
    h1p = pi3(h0p);
    h2p = pi3(h1p);

    // and then we check that the morphism is valid
    std::cout << "check for h(0) = " << h0 << ":" << std::endl;; 
    if(assertSquareFreeUnifMorph(h0, h1, h2)){
        std::cout << "  The morphism is square-free." << std::endl;
    }
    else{
        std::cout << "  The morphism is not square-free." << std::endl;
    }

    std::cout << "check for s^a(h(0))<"<< p << "> = " << h0p << ":" << std::endl; 
    if(assertSquareFreeUnifMorph(h0p, h1p, h2p)){
        std::cout << "  The morphism is square-free." << std::endl;
    }
    else{
        std::cout << "  The morphism is not square-free." << std::endl;
    }
    return 0;
}

