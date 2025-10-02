/*

Code to assert that for a fiven pair of integers (p, q), there is no infinite ternary square-free word that is square-free modulo p and q 

compilation : g++ -O3 -march=native -flto -g -o table_D_red.o table_D_red.cpp
execution   : ./table_D_red.o


*/



#include <iostream>
#include <algorithm>
#include <string.h>
#include <stack>
#include <cassert>
#include <time.h>
#include <chrono>
#include <unordered_map>
#include <set>
#include <fstream>
#include <iomanip>
#include <utility>
#include <random>
#include <deque>
#include <ctime>
#include <chrono>

#define P 8
#define Q 5
#define FAC_SIZE 40                         // should be the gcm of P and Q
#define SIZE 40000                   
#define EXTENDABILITY_LENGTH 80            // should be a multiple of FAC_SIZE
#define BLOCK_EXTENDABILITY_LENGTH 200      // should be a multiple of FAC_SIZE
#define LARGE_WORDS 240                     // length of large words

std::chrono::time_point<std::chrono::_V2::system_clock, std::chrono::duration<long int, std::ratio<1, 1000000000>>> start; 

std::string getSubsequence(const std::string& s, const int& p){
    std::string res = "";
    for(int i = 0; i < s.size(); i = i + p){
        res = res + s[i];
    }
    return res;
}

bool checkSquarefree(const std::string& s, bool verbose=false){
    /*
    Asserts that s is square-free modulo p.
    */
    int n = s.size();
    int cpt;
    // for every possible period
    for(int period = 1; period <= n/2; period++){
        cpt = 0;
        for(int pos = 0; pos < n - period; pos++){
            if(s[pos] == s[pos + period]){
                cpt++;
            }
            else{
                cpt = 0;
            }
            if(cpt == period){
                if(verbose){
                    std::cout << "--\n";
                    std::cout << s << " \nhas a square starting at pos : " << pos << " of period " << period << std::endl;
                    std::cout << "--\n";
                }
                return false;
            }
        }
    }
    return true;
}

bool checkSquareFreeAtEndString(const std::string& s){
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

bool checkSquareFreeAtEndV2String(const std::string& s, const int& p){
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

bool checkSquareFreeAtEndSuffixString(const std::string& s, const int& length){
    /*
    s = wu with |u| = length, and w and u are square-free. Is wu square-free ?
    */
    if(s.size() <= length)
        return true;
    for(int period = 1; period <= s.size()/2; period++){
        for(int end = 0; end < std::min(2*period - 1, length); end++){
            int end_pos = s.size() - length + end;
            bool flag = true;
            for(int i = 0; i < period; i++ ){             
                if(s[end_pos - i] != s[end_pos - i - period]){
                    flag = false;
                    break;
                }
            }
            if(flag){
                return false;
            }
        }
    }
    return true;
}

bool checkSquareFreeAtEndV2SuffixString(const std::string& s, const int& p, const int& length){
    /*
    s = wu with |u| = length, and w and u are square-free. Is wu square-free mod p ?
    also p devides |w| and |u|
    */
    int n = s.size();
    if(n <= length || (n)%p != 0){
        return true;
    }
    // For each possible position in u[p] :
    for(int end = 0; end < length; end = end + p){
        // For each possible period
        for(int period = (end/p + 1)/2 + 1; period <= (n - length + end + p)/(2*p); period++){
            // We check whether the word of length 2*period that ends at position end is a square
            bool flag = true;
            for(int i = 0; i < period; i++){
                if(s[n - length + end + p - 2*period*p + i*p] != s[n - length + end + p - period*p + i*p]){
                    flag = false;
                    break;
                }
            }
            if(flag){
                return false;
            }
        }
    }
    return true;
}

bool checkSquareFreeAtBegV2SuffixString(const std::string& s, const int& p, const int& length){
    /*
    s = uw with |u| = length, and w and u are square-free. Is uw square-free mod p ?
    also p devides |w| and |u|
    */
    int n = s.size();
    if(n <= length || (n)%p != 0){
        return true;
    }
    // For each possible position in u[p] :
    for(int end = 0; end < length; end = end + p){
        // For each possible period
        for(int period = (end/p + 1)/2 + 1; period <= (n - length + end + p)/(2*p); period++){
            // We check whether the word of length 2*period that ends at position end is a square
            bool flag = true;
            for(int i = 0; i < period; i++){
                if(s[length - end - p + period*p + i*p] != s[length - end - p + i*p]){
                    flag = false;
                    break;
                }
            }
            if(flag){
                return false;
            }
        }
    }
    return true;
}


bool is_left_extandable(const std::string& w){
    /*
    chack if w can be left extended into a word of length EXTENDABILITY_LENGTH
    */
    std::string s = w;
    std::stack<char> stack;     // Stack to emulate a DFS
    // Push on the stack depends on the alphabet : 
    stack.push('p');            
    stack.push('2'); 
    stack.push('1');                
    stack.push('0');
    // While there exists extensions, i.e. the stack is not empty
    while(!stack.empty()){
        // if the top of the stack is a p, then we checked all the extensions of the current string so we pop_back it 
        if(stack.top() == 'p'){
            stack.pop();
            s.erase(0, 1);
        }
        // Otherwise, we try a new extension
        else{        
            s = stack.top() + s;
            stack.pop();
            // Check conditions 
            if(checkSquarefree(s)){
                if(s.size()%FAC_SIZE == 0){
                    if(checkSquarefree(getSubsequence(s, P)) && checkSquarefree(getSubsequence(s, Q))){
                        if(s.size() >= 2*EXTENDABILITY_LENGTH - FAC_SIZE){
                            return true;
                        }
                        // If true, then we can extend s
                        stack.push('p');
                        stack.push('2'); 
                        stack.push('1');                
                        stack.push('0'); 
                    }
                    else{
                        // backtrack
                        s.erase(0, 1);
                    }
                }
                else{
                    stack.push('p');
                    stack.push('2'); 
                    stack.push('1');                
                    stack.push('0');
                }
            }
            else{
                // backtrack
                s.erase(0, 1);
            }
        }
    }
    return false;
}

bool is_extandable(const std::string& w){
    /*
    chack if w can be extended into a word of length EXTENDABILITY_LENGTH
    */
    std::string s = w;
    std::stack<char> stack;     // Stack to emulate a DFS
    // Push on the stack depends on the alphabet : 
    stack.push('p');            
    stack.push('2'); 
    stack.push('1');                
    stack.push('0');
    bool flag = true;
    // While there exists extensions, i.e. the stack is not empty
    while(!stack.empty()){
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
            if(checkSquareFreeAtEndString(s) && checkSquareFreeAtEndV2String(s, P) && checkSquareFreeAtEndV2String(s, Q)){
                if(s.size() >= EXTENDABILITY_LENGTH){
                    if(flag){
                        flag = false;
                        std::cout << "tentative of left extendability" << std::endl;
                    }
                    if(is_left_extandable(s)){
                        return true;
                    }
                    else{
                        s.pop_back();
                    }
                }
                else{
                    // If true, then we can extend s
                    stack.push('p');
                    stack.push('2'); 
                    stack.push('1');                
                    stack.push('0');
                }  
            }
            else{               
                // otherwise we backtrack
                s.pop_back();
            }
        }
    }
    return false;
}



std::vector<std::string> search_factors(){
    /*
    
    Generator of every word of size FAC_SIZE that are square-free, square-free modulo P and Q
    
    */
    std::cout << "Search for factor square-free, square-free mod " << P << " and " << Q << " of length " << FAC_SIZE << " and left and right extendable." << std::endl; 
    std::vector<std::string> all_factors = {};
    std::string s = "01";
    std::stack<char> stack;      // Stack to emulate a DFS
    // Push on the stack depends on the alphabet : 
    stack.push('p');          
    stack.push('2'); 
    stack.push('1');                
    stack.push('0');
    // While there exists extensions, i.e. the stack is not empty
    while(!stack.empty()){
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
            if(checkSquareFreeAtEndString(s) && checkSquareFreeAtEndV2String(s, P) && checkSquareFreeAtEndV2String(s, Q)){  
                // If true, then we can extend s
                if(s.size() == FAC_SIZE){
                    if(is_extandable(s)){
                        all_factors.push_back(s);
                        //std::cout << all_factors.size() << " good factors" << std::endl;
                    }
                    s.pop_back();
                    
                }
                else{
                    stack.push('p');
                    stack.push('2'); 
                    stack.push('1');                
                    stack.push('0');
                }
                
            }
            else{               
                // otherwise we backtrack
                s.pop_back();
            }
        }
    }
    std::cout << "Search done, " << all_factors.size() << " factors starting with \"01\" have been found which leads to a total of " << 6*all_factors.size() << " factors." << std::endl;
    return all_factors;
} 

std::vector<std::string> all_permutations(const std::string& s){
    /*
    s starts with 01

    returns its 6 equivalents words (starting with 01, 02, 10, 12, 20, 21) 
    */
    std::string s02, s10, s12, s20, s21;
    for(const auto c : s){
        if(c == '0'){
            s02 = s02 + "0";
            s10 = s10 + "1";
            s12 = s12 + "1";
            s20 = s20 + "2";
            s21 = s21 + "2";
        }
        else if(c == '1'){
            s02 = s02 + "2";
            s10 = s10 + "0";
            s12 = s12 + "2";
            s20 = s20 + "0";
            s21 = s21 + "1";
        }
        else{
            s02 = s02 + "1";
            s10 = s10 + "2";
            s12 = s12 + "0";
            s20 = s20 + "1";
            s21 = s21 + "0";
        }
    }
    return std::vector<std::string>({s, s02, s10, s12, s20, s21});
}

std::vector<std::string> all_words(std::vector<std::string> l){
    /*
    l must be a list of words all starting with 01
    */
    std::vector<std::string> res = {};
    for(const auto s : l){
        for(const auto e : all_permutations(s)){
            res.push_back(e);
        }
    }
    return res;
}

bool is_extandable_block(std::string& s, std::vector<std::string> nodes){
    /*
    Nodes is a list of "good" factors of lengfth FAC_LENGTH

    s = uv with u, v in nodes

    is s extendable do a word of length BLOCK_EXTENDABILITY_LENGTH square-free, square-free mod P and Q with only blocks from nodes ?
    */
    return true;
}

std::unordered_map<std::string, std::vector<std::string>> Rauzy(std::vector<std::string> nodes){
    std::unordered_map<std::string, std::vector<std::string>> rauzy;
    for(const auto n1 : nodes){
        rauzy[n1] = {};
        for(const auto n2 : nodes){
            if(checkSquarefree(n1+n2) && checkSquarefree(getSubsequence(n1 + n2, P)) && checkSquarefree(getSubsequence(n1 + n2, Q))){
                rauzy[n1].push_back(n2);
            }
        }
        //std::cout << "deg( " << n1 << " )  =  " << rauzy[n1].size() << std::endl;
    }
    return rauzy;
}


std::vector<std::string> long_words(const std::vector<std::string> nodes, std::unordered_map<std::string, std::vector<std::string>> rauzy, const int LARGE_WORDS_LENGTH){
    /*
    Computes all "large" ternary square-free words, square-free mod P and Q

    "larges" means of length ...
    */
    std::cout << "generation of every good words of length " << LARGE_WORDS_LENGTH << std::endl;
    std::vector<std::string> res = {};
    std::vector<std::string> all_nodes = all_words(nodes);
    // For every possible prefix of size 40 : 
    for(const auto node : nodes){
        std::string s = node;
        std::stack<std::string> stack;
        for(const auto nei : rauzy[node]){
            stack.push(nei);
        }
        while(!stack.empty()){
            if(stack.top() == "p"){
                stack.pop(); 
                s.erase(s.size() - FAC_SIZE , FAC_SIZE);
            }
            else{
                std::string local_node = stack.top();
                stack.pop();
                s = s + local_node;
                // checksquare-free end with size
                if(checkSquareFreeAtEndV2SuffixString(s, P, FAC_SIZE) && checkSquareFreeAtEndV2SuffixString(s, Q, FAC_SIZE) && checkSquareFreeAtEndSuffixString(s, FAC_SIZE)){
                    if(s.size() >= LARGE_WORDS_LENGTH){
                        // std::cout << " a large word has been found  : \n" << s << std::endl;
                        //std::cout << s << std::endl;
                        res.push_back(s);
                        s.erase(s.size() - FAC_SIZE , FAC_SIZE);
                    }
                    else{
                        stack.push("p");
                        for(const auto nei : rauzy[local_node]){
                            stack.push(nei);
                        }
                    }
                }
                else{
                    s.erase(s.size() - FAC_SIZE , FAC_SIZE);
                }
            }
        }
        // Here, we could delete node from the graph since we could not create a large word with node as prefix
    }
    std::cout << "Generation done, " << res.size() << " good words." << std::endl;
    return res; 
}



// Code big step rauzy as a graph

std::tuple<std::unordered_map<std::string, int>, std::vector<std::vector<int>>, std::vector<std::vector<int>>> graph_BigStepRauzy(const std::vector<std::string>& factors, const std::vector<std::string>& small_factors){
    /*
    for u, v in factors, if the suffix of size |u| - FAC_SIZE of u is the prefix of size |v| - FAC_SIZE of v, then add the edge uv to the graph

    factors must be "all factors"
    */
    std::set<std::string> set_of_factors(factors.begin(), factors.end());
    int size_of_factors = factors[0].size();
    // unordered_map : factor -> factor index
    std::unordered_map<std::string, int> toVertex;
    for(int i = 0; i < factors.size(); i++){
        toVertex[factors[i]] = i;
    }

    // adjacancy list
    std::vector<std::vector<int>> adjList(factors.size(), std::vector<int>{});
    std::vector<std::vector<int>> invAdjList(factors.size(), std::vector<int>{});

    // unordered_map of the suffixes and prefixes so we compute them just once
    std::unordered_map<std::string, std::string> suff;
    for(const auto factor : factors){ 
        std::string s = "";
        for(int i = 0; i < size_of_factors - FAC_SIZE; i++){
            s = s + factor[FAC_SIZE + i];
        }
        suff[factor] = s;
    }

    // construction of the graph
    int cpt = 0;
    std::string nei;
    for(const auto factor : factors){
        for(const auto extention : small_factors){
            nei = suff[factor] + extention;
            if(set_of_factors.find(nei) != set_of_factors.end()){
                adjList[toVertex[factor]].push_back(toVertex[nei]);
                invAdjList[toVertex[nei]].push_back(toVertex[factor]);
            }
        }
        cpt++;
        //std::cout << "factor " << factor << " done, " << cpt << " / " << factors.size() << " deg : " << adjList[toVertex[factor]].size() << std::endl;
    }
    return std::make_tuple(toVertex, adjList, invAdjList);
}

void statistics(std::unordered_map<std::string, int>& toVertex, std::vector<std::vector<int>>& adjList, std::vector<std::vector<int>>& invAdjList){
    int num_vertices = toVertex.size();
    int sum_degin = 0;
    int sum_degout = 0;
    int max_degin = 0;
    int max_degout = 0;
    int num_of_zeros_in = 0;
    int num_of_zeros_out = 0;
    for(int u = 0; u < num_vertices; u++){
        sum_degin = sum_degin + invAdjList[u].size();
        if(invAdjList[u].size() > max_degin){
            max_degin = invAdjList[u].size();
        }
        sum_degout = sum_degout + adjList[u].size();
        if(adjList[u].size() > max_degout){
            max_degout = adjList[u].size();
        }
        if(invAdjList[u].size() == 0){
            num_of_zeros_in++;
        }
        if(adjList[u].size() == 0){
            num_of_zeros_out++;
        }
    }
    std::cout << num_vertices << " vertices, max_degin = " << max_degin << ", max_degout = " << max_degout << ", sum_degin = " << sum_degin << ", sum_degout = " << sum_degout << ", num_of_zeros_in = " << num_of_zeros_in << ", num_of_zeros_out = " << num_of_zeros_out << std::endl;
}

void prunning_graph_BigStepRauzy(std::unordered_map<std::string, int>& toVertex, std::vector<std::vector<int>>& adjList, std::vector<std::vector<int>>& invAdjList){
    /*
    disconnects every node of out-degree or of indegree 0 until no new changes occur
    */
    bool flag = true;
    int num_vertices = toVertex.size();
    std::vector<bool> isolated(num_vertices, false);
    while(flag){
        flag = false;
        for(int u = 0; u < num_vertices; u++){
            // If degout(u) = 0 :
            if(adjList[u].size() == 0 && !isolated[u]){
                flag = true;
                // for every in-neighbour
                for(const auto v : invAdjList[u]){
                    // we remove the arrow vu from the graph
                    auto it = std::find(adjList[v].begin(), adjList[v].end(), u);
                    if(it != adjList[v].end()){
                        adjList[v].erase(it);
                    }
                }
                invAdjList[u] = {};
                isolated[u] = true;
            }
            if(invAdjList[u].size() == 0 && !isolated[u]){
                flag = true;
                // for every in-neighbour
                for(const auto v : adjList[u]){
                    // we remove the arrow uv from the graph
                    auto it = std::find(invAdjList[v].begin(), invAdjList[v].end(), u);
                    if(it != invAdjList[v].end()){
                        invAdjList[v].erase(it);
                    }
                }
                adjList[u] = {};
                isolated[u] = true;
            }
        }
    }
}

void print_factor(std::string& s, std::unordered_map<std::string, int>& toVertex, std::vector<std::vector<int>>& adjList, const int LARGE_WORDS_LENGTH){
    for(int i = 0; i < s.size() - LARGE_WORDS_LENGTH; i = i + FAC_SIZE){
        std::string loc = "";
        for(int j = 0; j < LARGE_WORDS_LENGTH; j++){
            loc = loc + s[i + j];
        }
        std::cout << "(" << toVertex[loc] << " | " << adjList[toVertex[loc]].size() << "°)  -   "; 
    }
    std::cout << std::endl;
}

int right_extends_with_graph_BigStepRauzy(const int& factor_id, std::string& factor, std::unordered_map<std::string, int>& toVertex, std::vector<std::vector<int>>& adjList, std::vector<std::string>& suffix, const int& TO , const int& LARGE_WORDS_LENGTH){
    std::cout << "try right extend of " << factor_id << " " << factor << std::endl;
    int TO_cpt = 0;
    // string for the search with factor as a prefix
    std::string s = factor;
    // stack for the backtrack
    std::stack<std::string> stack;
    for(const int nei : adjList[factor_id]){
        stack.push(suffix[nei]);
    }
    while(!stack.empty() && (TO == -1 || TO_cpt < TO)){
        TO_cpt++;
        if(TO_cpt % 10000000 == 0){
            auto end = std::chrono::high_resolution_clock::now();
            auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
            std::cout << "TIME FROM START = " << duration.count()/1000 << std::endl;
            std::cout << TO_cpt << std::endl;
            print_factor(s, toVertex, adjList, LARGE_WORDS_LENGTH); 
        } 
        // If we want to backtrack
        if(stack.top() == "p"){
            stack.pop();
            // we erase the FAC_SIZE last letters of s
            s.erase(s.size() - FAC_SIZE , FAC_SIZE);
        }
        else{
            std::string local_factor = stack.top();
            stack.pop();
            s = s + local_factor;
            // Now, we check that s is a good word
            //if(checkSquareFreeAtEndSuffixString(s, FAC_SIZE) && checkSquareFreeAtEndSuffixString(getSubsequence(s, P), FAC_SIZE/P) && checkSquareFreeAtEndSuffixString(getSubsequence(s, Q), FAC_SIZE/Q)){
            if(checkSquareFreeAtEndV2SuffixString(s, P, FAC_SIZE) && checkSquareFreeAtEndV2SuffixString(s, Q, FAC_SIZE)){
                if(s.size() >= SIZE){
                    std::cout << " a large word has been found  : \n" << s << std::endl;
                    return -1;
                }
                stack.push("p");
                std::string current_factor = "";
                for(int i = 0; i < LARGE_WORDS_LENGTH; i++){
                    current_factor = current_factor + s[s.size() - LARGE_WORDS_LENGTH + i];
                }
                for(const int nei : adjList[toVertex[current_factor]]){
                    stack.push(suffix[nei]);
                }
            }
            else{
                s.erase(s.size() - FAC_SIZE , FAC_SIZE);
            }
        }
    }
    return TO_cpt;
}

int left_extends_with_graph_BigStepRauzy(const int& factor_id, std::string& factor, std::unordered_map<std::string, int>& toVertex, std::vector<std::vector<int>>& invAdjList, std::vector<std::string>& prefix, const int& TO , const int& LARGE_WORDS_LENGTH){
    std::cout << "try left extend of " << factor_id << " " << factor << std::endl;
    int TO_cpt = 0;
    // string for the search with factor as a prefix
    std::string s = factor;
    // stack for the backtrack
    std::stack<std::string> stack;
    for(const int nei : invAdjList[factor_id]){
        stack.push(prefix[nei]);
    }
    while(!stack.empty() && (TO == -1 || TO_cpt < TO)){
        TO_cpt++;
        if(TO_cpt % 10000000 == 0){
            auto end = std::chrono::high_resolution_clock::now();
            auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
            std::cout << "TIME FROM START = " << duration.count()/1000 << std::endl;
            std::cout << TO_cpt << std::endl;
            print_factor(s, toVertex, invAdjList, LARGE_WORDS_LENGTH); 
        } 
        // If we want to backtrack
        if(stack.top() == "p"){
            stack.pop();
            // we erase the FAC_SIZE last letters of s
            s.erase(0 , FAC_SIZE); // erase the prefix of length FAC_SIZE
        }
        else{
            std::string local_factor = stack.top();
            stack.pop();
            s = local_factor + s;
            // Now, we check that s is a good word
            //if(checkSquareFreeAtEndSuffixString(s, FAC_SIZE) && checkSquareFreeAtEndSuffixString(getSubsequence(s, P), FAC_SIZE/P) && checkSquareFreeAtEndSuffixString(getSubsequence(s, Q), FAC_SIZE/Q)){
            if(checkSquareFreeAtBegV2SuffixString(s, P, FAC_SIZE) && checkSquareFreeAtBegV2SuffixString(s, Q, FAC_SIZE)){
                if(s.size() >= SIZE){
                    std::cout << " a large word has been found  : \n" << s << std::endl;
                    return -1;
                }
                stack.push("p");
                std::string current_factor = "";
                for(int i = 0; i < LARGE_WORDS_LENGTH; i++){
                    current_factor = current_factor + s[i];
                }
                for(const int nei : invAdjList[toVertex[current_factor]]){
                    stack.push(prefix[nei]);
                }
            }
            else{
                s.erase(0 , FAC_SIZE); // erase the prefix of length FAC_SIZE
            }
        }
    }
    return TO_cpt;
}

void searchTimeOut_with_graph_BigStepRauzy(std::vector<std::string>& factors, std::unordered_map<std::string, int>& toVertex, std::vector<std::vector<int>>& adjList, std::vector<std::vector<int>>& invAdjList, const int& LARGE_WORDS_LENGTH){
    /*
    backtrack algorithm using the Rauzy graph in the search

    */
    std::cout << "start search with big step Rauzy graph, LARGE_WORDS_LENGTH = " << LARGE_WORDS_LENGTH << std::endl;
    std::vector<std::string> all_factors = all_words(factors); // notice that all_factors is the inverse unordered_map of toVertex
    // We first precompute the suffixes of length FAC_SIZE
    std::vector<std::string> suffix; // stores the suffixs of lengths FAC_SIZE
    std::vector<std::string> prefix; // stores the prefixs of lengths FAC_SIZE
    for(const auto factor : all_factors){
        std::string su = "";
        std::string pr = "";
        for(int i = 0; i < FAC_SIZE; i++){
            su = su + factor[factor.size() - FAC_SIZE + i];
            pr = pr + factor[i];
        }
        suffix.push_back(su);
        prefix.push_back(pr);
    }
    int TO = 200;
    int fp_detector = 0;
    std::deque<int> fifo;
    for(int i = 0; i < factors.size(); i++){
        fifo.push_back(6*i);
    }
    while(fifo.size() > 0){
        std::cout << "|fifo| = " << fifo.size() << " TO = " << TO << " FP = " << fp_detector << std::endl;
        int factor_id = fifo.front();
        std::string factor = all_factors[factor_id];
        fifo.pop_front();
        int TO_cpt_right = right_extends_with_graph_BigStepRauzy(factor_id, factor, toVertex, adjList, suffix, TO, LARGE_WORDS_LENGTH);
        int TO_cpt_left;
        if(TO_cpt_right >= TO){
            TO_cpt_left = left_extends_with_graph_BigStepRauzy(factor_id, factor, toVertex, invAdjList, prefix, TO, LARGE_WORDS_LENGTH);
        }
       else{
            TO_cpt_left = TO_cpt_right;
       }
        // if the backtrack ended before the Time Out
        std::cout << TO_cpt_left << " " << TO_cpt_right << std::endl;
        if(TO_cpt_right == -1 || TO_cpt_left == -1){
            return;
        }
        else if(TO_cpt_right < TO || TO_cpt_left < TO){
            // we reset the counter for detection of fixed point
            fp_detector = 0;
            // we can cut the graph by removing factor (and the equivalent fact)
            for(const auto factor_to_delete : all_words({factor})){
                // we remove factore_to_delete from the neighbourhood of its right neighbours
                for(const auto right_nei : adjList[toVertex[factor_to_delete]]){
                    auto it = std::find(invAdjList[right_nei].begin(), invAdjList[right_nei].end(), toVertex[factor_to_delete]);
                    if(it != invAdjList[right_nei].end()){
                        invAdjList[right_nei].erase(it);
                    }
                }
                adjList[toVertex[factor_to_delete]] = {};
                //toVertex.erase(factor_to_delete);
            }
            prunning_graph_BigStepRauzy(toVertex, adjList, invAdjList);
            statistics(toVertex, adjList, invAdjList);
            // we can finally remove factor from the unordered_maps
        }
        // otherwise
        else{
            // we put factor at the end of the deque
            fifo.push_back(factor_id);
        }
        fp_detector++;
        if(fp_detector > fifo.size()){
            statistics(toVertex, adjList, invAdjList);
            if(TO == 200){
                return;
                TO = 30000000;
            }
            else{
                TO = -1;
            }
            fp_detector = 0;
            std::random_device rd;
            std::mt19937 g(rd());
            std::shuffle(fifo.begin(), fifo.end(), g);
        }
    }
    std::cout << "computation ended." << std::endl;
}



int main(int argc, char* argv[]){ 
    start = std::chrono::high_resolution_clock::now();
    // std::vector<std::string> l = search_factors(); // with EXTENDABILITY_LENGTH = 80 takes about 14min 
    std::vector<std::string> l = {  "0102012021012102010210121021201020120212",
                                    "0102012021012102010210121021201020121012",
                                    "0102012021012102010210121021201020121020",
                                    "0102012021020102101201020120212010201210",
                                    "0102012021020102101201020120212012102010",
                                    "0102012021020102101201020120212012102120",
                                    "0102012021020102101201020121012021012102",
                                    "0102012021020102101202120102012021012102",
                                    "0102012021020102101202120121012010201210",
                                    "0102012021020102101202120121012021012102",
                                    "0102012021020102101210212012101201021012",
                                    "0102012021020102101210212012101201021202",
                                    "0102012021201020121020102101210201210120",
                                    "0102012021201020121020102120210201210120",
                                    "0102012021201020121021202101210201210120",
                                    "0102012021201020121021202102012101201020",
                                    "0102012021201020121021202102012101202120",
                                    "0102120210120102012101202101210201021012",
                                    "0102120210120102012101202101210201021202",
                                    "0102120210120102012101202102010210120102",
                                    "0102120210120102012101202102010210120212",
                                    "0102120210120212010201202101210201021012",
                                    "0102120210120212010201202101210201021202",
                                    "0102120210120212010201202102010210120102",
                                    "0102120210120212010201202102010210120212",
                                    "0102120210120212012101202101210201021012",
                                    "0102120210120212012101202101210201021202",
                                    "0102120210120212012101202102010210120102",
                                    "0102120210120212012101202102010210120212",
                                    "0120210121020102101210212012101201021012",
                                    "0120210121020102101210212012101201021202",
                                    "0120210121021202102010212012101201021012",
                                    "0120210121021202102010212012101201021202",
                                    "0120212010201210212021020102120210120102",
                                    "0120212010201210212021020102120210120212",
                                    "0120212010201210212021020121012021201020",
                                    "0120212010201210212021020121012021201210",
                                    "0121012010201210212021020102120210120102",
                                    "0121012010201210212021020102120210120212",
                                    "0121012010201210212021020121012021201020",
                                    "0121012010201210212021020121012021201210",
                                    "0121012021012102010210121021201020120212",
                                    "0121012021012102010210121021201020121012",
                                    "0121012021012102010210121021201020121020",
                                    "0121012021020102101201020120212010201210",
                                    "0121012021020102101201020120212012102010",
                                    "0121012021020102101201020120212012102120",
                                    "0121012021020102101201020121012021012102",
                                    "0121012021020102101202120102012021012102",
                                    "0121012021020102101202120121012010201210",
                                    "0121012021020102101202120121012021012102",
                                    "0121012021020102101210212012101201021012",
                                    "0121012021020102101210212012101201021202",
                                    "0121012021201020121020102101210201210120",
                                    "0121012021201020121020102120210201210120",
                                    "0121012021201020121021202101210201210120",
                                    "0121012021201020121021202102012101201020",
                                    "0121012021201020121021202102012101202120",
                                    "0121020121012021020102101210201021201020",
                                    "0121020121012021020102101210201021201210"};
    std::cout << "|good factors| = " << l.size() << std::endl;
    // //search(l, Rauzy(all_words(l)));
    std::cout << "generation of medium length good words from the blocks" << std::endl;
    int LARGE_WORDS_LENGTH = 240;
    std::vector<std::string> lw = long_words(l, Rauzy(all_words(l)), LARGE_WORDS_LENGTH); // ~1min30sec with LARGE_WORDS_LENGTH = 320
    std::random_shuffle(lw.begin(), lw.end());
    std::cout << lw.size() << std::endl;
    std::tuple<std::unordered_map<std::string, int>, std::vector<std::vector<int>>, std::vector<std::vector<int>>> G =  graph_BigStepRauzy(all_words(lw), all_words(l));
    std::unordered_map<std::string, int> toVertex = std::get<0>(G); 
    std::vector<std::vector<int>> adjList = std::get<1>(G); 
    std::vector<std::vector<int>> invAdjList = std::get<2>(G);
    std::cout << " statistics pre prunning : " << std::endl;
    statistics(toVertex, adjList, invAdjList);
    std::cout << "prunning." << std::endl;
    prunning_graph_BigStepRauzy(toVertex, adjList, invAdjList);
    std::cout << " statistics post prunning : " << std::endl;
    statistics(toVertex, adjList, invAdjList);
    searchTimeOut_with_graph_BigStepRauzy(lw, toVertex, adjList, invAdjList, LARGE_WORDS_LENGTH);

    return 0;
}
