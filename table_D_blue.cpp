#include <iostream>
#include <fstream>
#include <iomanip>
#include "utility.hpp"

/*

Code that verifies that there exists a morphism that is square-free, square-free modulo p and square-free modulo q
for all pairs (p, q) with p < q and (p, q) not in      
    (3, 4) (3, 5) (3, 6) (3, 7) (3, 8) (3, 10)
    (4, 5) (4, 6) (4, 7) (4, 8) (4, 9) (4, 10) (4, 14) 
    (5, 10) 
    (6, 7) (6, 9) (6, 12) 
    (7, 14) 
    (8, 12) (8, 16) 
    (9, 18) 
    (10, 15) (10, 20) 
    (12, 18)

To do so, we check that the solutions stored in table_D_morphisms.txt are valid

compilation : g++ -O3 -o table_D_blue.o table_D_blue.cpp
execution   : ./table_E.o

*/

void usage(int i){
    if(i == 0) std::cout << "Wrong number of parameters." << std::endl;
    else std::cout << "Wrong pair." << std::endl;
    std::cout << "Usage 1 : ./table_E.cpp <p> <q> (with 3 <= p < q <= 20 and (p, q) not in {(3, 4) (3, 5) (3, 6) (3, 7) (3, 8) (3, 10) " <<
                                                                                            "(4, 5) (4, 6) (4, 7) (4, 8) (4, 9) (4, 10) (4, 14) " <<
                                                                                            "(5, 10) " << 
                                                                                            "(6, 7) (6, 9) (6, 12) " << 
                                                                                            "(7, 14) " << 
                                                                                            "(8, 12) (8, 16) " <<
                                                                                            "(9, 18) " <<  
                                                                                            "(10, 15) (10, 20) " << 
                                                                                            "(12, 18)}" << std::endl;
}

int main(int argc, char* argv[]){
    if(argc != 3){
        usage(0);
        return 1;
    }
    int p = std::stoi(argv[1]);
    int q = std::stoi(argv[2]);

    // We read the morphism from table_D_morphisms.txt
    std::ifstream file("table_D_morphisms.txt");
    std::string line;
    int pr=0, qr=0;
    for (int lineNum = 1; std::getline(file, line); ++lineNum) {
            std::istringstream iss(line);
            iss >> pr >> qr;
            if(iss.fail()) continue;
            if(pr == p && qr == q) break;
    }
    if(pr != p || qr != q){
        usage(1);
        return 1;
    }

    std::string h0, h1, h2, h0p, h1p, h2p, h0q, h1q, h2q;
    std::getline(file, h0);
    std::getline(file, h1);
    std::getline(file, h2);
    h0p = subsequence(h0, p, 0);
    h0q = subsequence(h0, q, 0);

    h1p = subsequence(h1, p, 0);
    h1q = subsequence(h1, q, 0);

    h2p = subsequence(h2, p, 0);
    h2q = subsequence(h2, q, 0);

    // and then we check that the morphism is valid
    std::cout << "Check for h:" << std::endl;; 
    if(assertSquareFreeUnifMorph(h0, h1, h2)){
        std::cout << "  The morphism h is square-free." << std::endl;
    }
    else{
        std::cout << "  The morphism h is not square-free." << std::endl;
    }

    std::cout << "Check for h<"<< p << ">:" << std::endl; 
    if(assertSquareFreeUnifMorph(h0p, h1p, h2p)){
        std::cout << "  The morphism h<"<< p << "> is square-free." << std::endl;
    }
    else{
        std::cout << "  The morphism h<"<< p << "> is not square-free." << std::endl;
    }

    std::cout << "Check for h<"<< q << ">:" << std::endl; 
    if(assertSquareFreeUnifMorph(h0q, h1q, h2q)){
        std::cout << "  The morphism h<"<< q << "> is square-free." << std::endl;
    }
    else{
        std::cout << "  The morphism h<"<< q << "> is not square-free." << std::endl;
    }
    return 0;
}

