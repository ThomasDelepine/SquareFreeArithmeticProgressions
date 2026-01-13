#include <iostream>
#include <fstream>
#include <iomanip>
#include <algorithm>
#include <string.h>


/*

Code that verifies that there exists a morphism that is square-free and square-free modulo p 

To do so, we check that the solutions stored in table_E_morphisms. are valid

compilation : g++ -O3 -o table_E.o table_E.cpp
execution   : ./table_E.o

*/

bool checkSquareFree(const std::string& s){
    /*
    verifies that s is square-free
    */
    for(size_t i = 0; i < s.size() - 1; i++){
        for(size_t period = 1; 2*period+i <= s.size(); period++){
            bool flag = false;
            for(size_t j = i; j < i + period; j++){
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

bool assertSquareFreeCircMorph(const std::string& h0){
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
    for(const std::string& s : {h0+h1+h2, h0+h1+h0, h0+h2+h0, h0+h2+h1}){
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
    for(size_t i = alpha; i < s.size(); i = i + p){
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

    // We read the morphism from table_E_morphisms.txt
    std::ifstream file("table_E_morphisms.txt");
    std::string line, h0, h0p;
    int pr=0, alpha;
    for (int lineNum = 1; std::getline(file, line); ++lineNum) {
            std::istringstream iss(line);
            iss >> pr >> h0 >> alpha;
            if(iss.fail()) continue;
            if(pr == p) break;
    }
    if(p != pr){
        std::cout << "Usage 2 : ./table_E.cpp <p> (with p in {3,4,5,7,8,9,10,11,12,14,15,16,20,21,22})" << std::endl;
        return 1;
    }

    h0p = subsequence(h0, p, alpha);

    // and then we check that the morphism is valid
    std::cout << "check for h(0) = " << h0 << ":" << std::endl;; 
    if(assertSquareFreeCircMorph(h0)){
        std::cout << "  The morphism is square-free." << std::endl;
    }
    else{
        std::cout << "  The morphism is not square-free." << std::endl;
    }

    std::cout << "check for s^a(h(0))<"<< p << "> = " << h0p << ":" << std::endl; 
    if(assertSquareFreeCircMorph(h0p)){
        std::cout << "  The morphism is square-free." << std::endl;
    }
    else{
        std::cout << "  The morphism is not square-free." << std::endl;
    }
    return 0;
}

