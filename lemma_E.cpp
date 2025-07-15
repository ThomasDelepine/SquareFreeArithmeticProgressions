

/*

Code to assert that for a fiven pair of integers (p, q), there is no infinite ternary square-free word that is square-free modulo p and q 

compilation : g++ -O3 -march=native -flto -g -o lemma_E.o lemma_E.cpp
execution   : ./lemma_E.o


*/



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


bool checkSquareFreeAtEnd(const std::string& s){
    /*
        Assumes s.pop_back() is square free
        Return true iff s is square free
    */ 
    int n = s.size();
    for(int i = 1; i <= n/2; i++){
        // Checks if there is a square of period i at the end of s
        bool flag = true;
        for(int j = 0; j < i; j++){
            // certificate that s does not end with a square of period i
            if(s[n - 1 - j] != s[n - 1 - i - j]){
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

bool checkSquareFreeAtEndV2(const std::string& s, const int p){
    /*
        Assumes s.pop_back()_p is square free and p >= 1
        Return true iff s_p is square free
        where w_p is the word w0wpw2p...
    */ 
    int n = s.size();
    // if the new symbol is not at a position k*p, then we don't need to check anything 
    if((n-1)%p != 0){
        return true;
    }
    for(int i = 1; i <= (n+p+1)/p/2; i++){
        // Checks if there is a square of period i at the end of s
        bool flag = true;
        for(int j = 0; j < i; j++){
            // certificate that s does not end with a square of period i
            if(s[n - 1 - p*j] != s[n - 1 - p*i - p*j]){
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


void search(const int p, const int q){
	/*
	Tries to construct the largerst possible ternary word that is square-free modulo p and square-free modulo q

	The search algorithm is a dfs-like exploring words following the lexicographic ordering

	Notice that, up to permutations of letters, it is enough to consider words starting with 01
	*/
    std::cout << "\nThe search for p = " << p << ", q = " << q << " started" << std::endl;
    std::string s = "001";       // Init of the string
    std::stack<char> stack;  	// Stack to emulate a DFS
    // Push on the stack depends on the alphabet : 
    stack.push('p');			
    stack.push('2'); 
    stack.push('1');                
    stack.push('0');
    // While there exists extensions, i.e. the stack is not empty
    while(!stack.empty()){
    	// std::cout << s << std::endl;
    	// if the top of the stack is a p, then we checked all the extensions of the current string so we pop_back it 
        if(stack.top() == 'p'){
            stack.pop();
            s.pop_back();
        }
        // Otherwise, we try a new extension
        else{        
            s = s + stack.top();
            stack.pop();
            // Check conditions 
            if(checkSquareFreeAtEndV2(s, p) && checkSquareFreeAtEndV2(s, q)){  
                // If true, then we can extend s
                if(s.size()%p == 0 || s.size()%q == 0 ){
                    stack.push('p');
                    stack.push('2'); 
                    stack.push('1');                
                    stack.push('0');
                }
                // Since we do not care about the whole word, we can assume that any position that is not a multiple of 2 or 3 contains a 0.
                else{
                    stack.push('p');
                    stack.push('0');
                }
                
                
            }
            else{               
            	// otherwise we backtrack
                s.pop_back();
            }
        }
    }
    std::cout << "\nThe search for p = " << p << ", q = " << q << " finished.\nThere is no infinite ternary word that is square-free modulo " << p << " and " << q << std::endl;
} 


int main() {
    int p = 2;
    int q = 3;
   	search(p, q);
    return 0;
}