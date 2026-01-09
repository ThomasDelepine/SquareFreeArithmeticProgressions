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

#define SIZE 1000000

/*

Code to assert that for a fiven pair of integers (p, q), there is no infinite ternary square-free word that is square-free modulo p and q 

compilation : g++ -O3 -march=native -flto -g -o table_D_red.o table_D_red.cpp
execution   : ./table_D_red.o


*/



bool checkSquareFreeAtEnd(const int (&s)[SIZE], const int& size, const int& p){
    /*
        Assumes s.pop_back()_p is square free and p >= 1
        Return true iff s_p is square free
        where w_p is the word w0wpw2p...
    */ 
    // if the new symbol is not at a position k*p, then we don't need to check anything 
    if((size-1)%p != 0){
        return true;
    }
    for(int i = 1; i <= (size+p+1)/p/2; i++){
        // Checks if there is a square of period i at the end of s
        bool flag = true;
        for(int j = 0; j < i; j++){
            // certificate that s does not end with a square of period i
            if(s[size - 1 - p*j] != s[size - 1 - p*i - p*j]){
                flag = false;
                break;
            }
        } 
        // If we just detected a square
        if(flag)
            return false;
    }
    return true;
}

bool no_infinite_square_free_word_mod_1_p_q(const int& p, const int& q){
    /*

    Asserts than there is no infinite ternary word square-free, square-free modulo p and square-free modulo q
    
    If for (p, q), an infinite such word exists, this fonction does not terminate

    The search algorithm is dfs-like and explores the set of ternary words following the lexicographic ordering

    Notice that, up to permutations of letters, it is enough to consider words starting with 01
    */

    // if p = 2 or q = 2 or (p, q) is on the (t, 2t) or (2t, t) or (2t, 3t) or (3t, 2t) line, we can return true
    if(p == 2 || q == 2 || p == 2*q || q == 2*p || 2*p == 3*q || 3*p == 2*q) return true;

    // Otherwise, we start a backtracking search
    int s[SIZE];       // Init of the string
    s[0] = 0;
    s[1] = 1;
    int size = 2;
    std::stack<int> stack;      // Stack to emulate a DFS
    // Push on the stack depends on the alphabet : 
    stack.push(3);          
    stack.push(2); 
    stack.push(1);                
    stack.push(0);
    // While there exists extensions, i.e. the stack is not empty
    while(!stack.empty()){
        //std::cout << s << std::endl;
        // if the top of the stack is a p, then we checked all the extensions of the current string so we pop_back it 
        if(stack.top() == 3){
            stack.pop();
            size--;
        }
        // Otherwise, we try a new extension
        else{        
            s[size] = stack.top();
            size++;
            stack.pop();
            // Check conditions 
            if(checkSquareFreeAtEnd(s, size, 1) && checkSquareFreeAtEnd(s, size, p) && checkSquareFreeAtEnd(s, size, q)){  
                // If true, then we can extend s
                stack.push(3);
                stack.push(2); 
                stack.push(1);                
                stack.push(0);                
            }
            else{               
                // otherwise we backtrack
                size--;
            }
        }
    }
    return true;
} 


int main(int argc, char* argv[]) {
    if (argc != 3) {
        std::cerr << "Usage: " << argv[0] << " <int1> <int2>" << std::endl;
        return 1;
    }
    int p = std::stoi(argv[1]);
    int q = std::stoi(argv[2]);
    //std::cout << "The search for p = " << p << ", q = " << q << " started" << std::endl;
    std::cout << "Search for an infinite ternary square-free word, square-free modulo " << p << " and modulo " << q << ": ";
    std::cout.flush();
    bool flag = no_infinite_square_free_word_mod_1_p_q(p, q);
    if(flag){
        std::cout << "no such word exists" << std::endl;
    }
    return 0;
}