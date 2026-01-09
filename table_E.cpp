#include <iostream>
#include <algorithm>
#include <string.h>
#include <stack>
#include <cassert>
#include <time.h>
#include <chrono>
#include <map>
#include <fstream>
#include <iomanip>
#include <utility>
#include <cassert>


/*

Code to assert that there exists a morphism that is square-free and square-free modulo p 

To do so, we check that the solutions stored in table_E_morphisms. are valid

compilation : g++ -O3 -march=native -flto -g -o table_E.o table_E.cpp
execution   : ./table_E.o

*/

bool checkSquareFree(std::string s){
    /*
    asserts that s is square-free
    */
    for(int i = 0; i < s.size() - 1; i++){
        for(int period = 1; period <= (s.size() - i)/2; period++){
            bool flag = false;
            for(int j = i; j < i + period; j++){
                if(s[j] != s[j + period]){
                    flag = true;
                    break;
                }
            }
            if(!flag){
                return false;
            }
        }
    }
    return true;
}

bool assertSquareFreeCircMorph(std::string h0){
    std::string h1, h2;
    for(const char e : h0){
        if(e == '0'){
            h1 = h1 + '1';
            h2 = h2 + '2';
        }
        else if(e == '1'){
            h1 = h1 + '2';
            h2 = h2 + '0';
        }
        else{
            h1 = h1 + '0';
            h2 = h2 + '1';
        }
    }
    for(std::string s : {h0+h1+h2, h0+h1+h0, h0+h2+h0, h0+h2+h1}){
        if(!checkSquareFree(s)){
            return false;
        }
    }
    return true;
}

std::string subsequence(const std::string& s, const int p, const int alpha){
    /*
    returns the subsequence congruent to alpha modulo p of s
    */
    std::string res = "";
    for(int i = alpha; i < s.size(); i = i + p){
        res = res + s[i];
    }
    return res;
}

int main(int argc, char* argv[]){
    if(argc != 2){
        std::cout << "Usage 1 : ./table_E.cpp <p> (with p in {3,4,5,7,8,9,10,11,12,14,15,16,20,21,22})" << std::endl;
        return 1;
    }
    int p = std::stoi(argv[1]);
    if(p != 3 && p != 4 && p != 5 && p != 7 && p != 8 && p != 9 && p != 10 && p != 11 && p != 12 && p != 14 && p != 15 && p != 16 && p != 20 && p != 21 && p != 22){
        std::cout << "Usage 2 : ./table_E.cpp <p> (with p in {3,4,5,7,8,9,10,11,12,14,15,16,20,21,22})" << std::endl;
        exit(0);
    }

    // We read the morphism from table_E_morphisms.txt
    std::ifstream file("table_E_morphisms.txt");
    std::string line, h0, h0p, alpha_s;
    for (int lineNum = 1; std::getline(file, line); ++lineNum) {
        if (lineNum == p) {
            std::istringstream iss(line);
            iss >> h0 >> alpha_s;
            break;
        }
    }
    int alpha = std::stoi(alpha_s);
    h0p = subsequence(h0, p, alpha);

    // and then we check that the morphism is valid
    std::cout << "check for h(0) = " << h0 << ":" << std::endl;; 
    if(assertSquareFreeCircMorph(h0)){
        std::cout << "  The morphis is square-free." << std::endl;
    }
    else{
        std::cout << "  The morphism is not square-free." << std::endl;
    }

    std::cout << "check for s^a(h(0))<"<< p << "> = " << h0p << ":" << std::endl; 
    if(assertSquareFreeCircMorph(h0p)){
        std::cout << "  The morphis is square-free." << std::endl;
    }
    else{
        std::cout << "  The morphism is not square-free." << std::endl;
    }
    return 0;
}

