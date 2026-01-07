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

Code to assert that for a fiven pair of integers (p, q), there is no infinite ternary square-free word that is square-free modulo p and q 

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

void assertSquareFreeCircMorph(std::string h0){
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
            std::cout << "not OK !" << std::endl;
            return;
        }
    }
    std::cout << "Square-free circular morphism !" << std::endl;
}

int main(int argc, char* argv[]){
    if(argc != 2){
        std::cout << "Usage 1 : ./table_E.cpp p (with p in {3,4,5,7,8,9,10,11,12,14,15,16,20,21,22})" << std::endl;
        return 1;
    }
    int p = std::stoi(argv[1]);
    std::ifstream file("table_E_morphisms.txt");
    if(p != 3 && p != 4 && p != 5 && p != 7 && p != 8 && p != 9 && p != 10 && p != 11 && p != 12 && p != 14 && p != 15 && p != 16 && p != 20 && p != 21 && p != 22){
        std::cout << "Usage 2 : ./table_E.cpp p (with p in {3,4,5,7,8,9,10,11,12,14,15,16,20,21,22})" << std::endl;
        exit(0);
    }
    std::string line;
    for (int i = 1; i <= p;  ++i) {
        std::getline(file, line);
    }
    //std::cout << line << std::endl;
    int k, alpha;
    std::string h0;
    int tmp = 3;
    while(line[tmp] != ' '){
        h0 = h0 + line[tmp];
        tmp++;
    }
    k = (line[h0.size() + 4] - '0')*10 + (line[h0.size() + 5] - '0');
    alpha = (line[h0.size() + 7] - '0');
    std::string h0p = "";
    for(int i = alpha; i < h0.size(); i = i + p){
        h0p = h0p + h0[i];
    }
    //std::cout << h0 << " " << alpha << " " << k << std::endl;
    //std::cout << h0p << std::endl;
    std::cout << "check for h(0) = " << h0 << std::endl; 
    assertSquareFreeCircMorph(h0);
    std::cout << "check for s^a(h(0))<"<< p << "> = " << h0p << std::endl;
    assertSquareFreeCircMorph(h0p);
    return 0;
}

