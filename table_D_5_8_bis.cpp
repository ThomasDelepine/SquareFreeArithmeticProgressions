

/*

Code to assert that for a fiven pair of integers (p, q), there is no infinite ternary square-free word that is square-free modulo p and q 

compilation : g++ -O3 -march=native -flto -g -o table_D_5_8_bis.o table_D_5_8_bis.cpp
execution   : ./table_D_5_8_bis.o


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
#define FAC_SIZE 40                         // should be the lcm of P and Q
#define SIZE 40000                   
#define EXTENDABILITY_LENGTH 80             // should be a multiple of FAC_SIZE
#define BLOCK_EXTENDABILITY_LENGTH 200      // should be a multiple of FAC_SIZE
#define LARGE_WORDS 240                     // length of large words

std::chrono::time_point<std::chrono::_V2::system_clock, std::chrono::duration<long int, std::ratio<1, 1000000000>>> start; 
std::vector<std::vector<int>>squaresEndP(2000, std::vector<int>(2000, 0));
std::vector<std::vector<int>>squaresEndQ(2000, std::vector<int>(2000, 0));
std::vector<int>squaresEndPath(2000, 0);
std::vector<std::vector<int>>squaresBegP(2000, std::vector<int>(2000, 0));
std::vector<std::vector<int>>squaresBegQ(2000, std::vector<int>(2000, 0));
std::vector<int>squaresBegPath(2000, 0);
std::vector<int> histo_length_right(4000, 0);
std::vector<int> histo_length_left(4000, 0);

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

bool checkSquareFreeAtEndVector(const std::deque<int>& s, const bool verbose=false){
    /*
        Assumes s.pop_back() is square free
        Return true iff s is square free
    */ 
    int n = s.size();
    if(n <= 1) return true;
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
        if(flag){
            if(verbose){squaresEndPath[i]++;}
            return false;
        }
    }
    return true;
}

bool checkSquareFreeAtBegVector(const std::deque<int>& s, const bool verbose=false){
    /*
        Assumes s.pop_front() is square free
        Return true iff s is square free
    */ 
    int n = s.size();
    if(n <= 1) return true;
    for(int i = 1; i <= n/2; i++){
        // Checks if there is a square of period i at the beginning of s
        bool flag = true;
        for(int j = 0; j < i; j++){
            // certificate that s does not start with a square of period i
            if(s[j] != s[i + j]){
                flag = false;
                break;
            }
        } 
        // If we just detected a square
        if(flag){
            if(verbose){squaresBegPath[i]++;}
            return false;
        }
    }
    return true;
}

bool checkSquareFreeAtEndV2String(const std::string& s, const int p){
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
    for(int i = 1; i <= (n+p-1)/(2*p); i++){
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

bool checkSquareFreeAtEndSuffixString(const std::string& s, const int length){
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

bool checkSquareFreeAtEndV2SuffixString(const std::string& s, const int p, const int length, const bool verbose=false){
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
                if(verbose){
                    if(p == P){squaresEndP[period][end]++;}
                    if(p == Q){squaresEndQ[period][end]++;}
                }
                return false;
            }
        }
    }
    return true;
}

bool checkSquareFreeForcedPositionsEnd(const std::string& s, const int p, const int length, const std::vector<std::pair<int, int>>& ForcedPositions){
    /*
    s = wu with |u| = length, and w and u are square-free. 
    check if some subwords of s are square-free
    */
    int n = s.size();
    if(n <= length || (n)%p != 0){
        return true;
    }
    // For each asked subwords:
    for(const auto e : ForcedPositions){
        int period = e.first;
        int end = e.second;
        // We check whether the word of length 2*period that ends at position end is a square
        bool flag = true;
        for(int i = 0; i < period; i++){
            if(s[n - length + end + p - 2*period*p + i*p] != s[n - length + end + p - period*p + i*p]){
                flag = false;
                break;
            }
        }
        if(flag){
            if(p == P){squaresEndP[period][end]++;}
            if(p == Q){squaresEndQ[period][end]++;}
            return false;
        }
    }
    return true;
}

bool checkSquareFreeAtBegV2SuffixString(const std::string& s, const int p, const int length, const bool verbose = false){
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
                if(verbose){
                    if(p == P){squaresBegP[period][end]++;}
                    if(p == Q){squaresBegQ[period][end]++;}
                }
                return false;
            }
        }
    }
    return true;
}

bool checkSquareFreeForcedPositionsBeg(const std::string& s, const int p, const int length, const std::vector<std::pair<int, int>>& ForcedPositions){
        int n = s.size();
    if(n <= length || (n)%p != 0){
        return true;
    }
    // For each asked subwords:
    for(const auto e : ForcedPositions){
        int period = e.first;
        int end = e.second;
        // We check whether the word of length 2*period that ends at position end is a square
        bool flag = true;
        for(int i = 0; i < period; i++){
            if(s[length - end - p + period*p + i*p] != s[length - end - p + i*p]){
                flag = false;
                break;
            }
        }
        if(flag){
            if(p == P){squaresBegP[period][end]++;}
            if(p == Q){squaresBegQ[period][end]++;}
            return false;
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
    std::vector<std::vector<char>> permutations = {{'0', '1', '2'}, {'0', '2', '1'}, {'1', '0', '2'}, {'1', '2', '0'}, {'2', '0', '1'}, {'2', '1', '0'}};
    std::vector<std::string>res(6, "");
    for(const auto c : s){
        for(int i = 0; i < 6; i++){
            res[i] += permutations[i][c - '0'];
        }
    }
    return res;
}

std::vector<std::string> all_words(const std::vector<std::string>& l){
    /*
    l must be a list of words all starting with 01
    */
    std::vector<std::string> res = {};
    for(const auto& s : l){
        for(const auto& e : all_permutations(s)){
            res.push_back(e);
        }
    }
    return res;
}

std::unordered_map<std::string, std::vector<std::string>> Rauzy(std::vector<std::string>& nodes){
    std::unordered_map<std::string, std::vector<std::string>> rauzy;
    for(const auto& n1 : nodes){
        rauzy[n1] = {};
        for(const auto& n2 : nodes){
            if(checkSquarefree(n1+n2) && checkSquarefree(getSubsequence(n1 + n2, P)) && checkSquarefree(getSubsequence(n1 + n2, Q))){
                rauzy[n1].push_back(n2);
            }
        }
        //std::cout << "deg( " << n1 << " )  =  " << rauzy[n1].size() << std::endl;
    }
    return rauzy;
}


std::vector<std::string> long_words(const std::vector<std::string>& nodes, std::unordered_map<std::string, std::vector<std::string>>& rauzy, const int LARGE_WORDS_LENGTH){
    /*
    Computes all "large" ternary square-free words, square-free mod P and Q

    "larges" means of length ...
    */
    std::cout << "generation of every good words of length " << LARGE_WORDS_LENGTH << std::endl;
    std::vector<std::string> res = {};
    std::vector<std::string> all_nodes = all_words(nodes);
    // For every possible prefix of size 40 : 
    for(const auto& node : nodes){
        std::string s = node;
        std::stack<std::string> stack;
        for(const auto& nei : rauzy[node]){
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
                        for(const auto& nei : rauzy[local_node]){
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
    for(const auto& factor : factors){ 
        std::string s = "";
        for(int i = 0; i < size_of_factors - FAC_SIZE; i++){
            s = s + factor[FAC_SIZE + i];
        }
        suff[factor] = s;
    }

    // construction of the graph
    int cpt = 0;
    std::string nei;
    for(const auto& factor : factors){
        for(const auto& extention : small_factors){
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

void statistics(const std::unordered_map<std::string, int>& toVertex, const std::vector<std::vector<int>>& adjList, const std::vector<std::vector<int>>& invAdjList){
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
    std::cout << num_vertices << " vertices, max_degin = " << max_degin << ", max_degout = " << max_degout << ", sum_degin = " << sum_degin << ", sum_degout = " << sum_degout << ", num_of_zeros_in = " << num_of_zeros_in << ", num_of_zeros_out = " << num_of_zeros_out << " mean deg = " << sum_degout/(num_vertices - num_of_zeros_out + 0.0) << std::endl;
}

void prunning_graph_BigStepRauzy(const std::unordered_map<std::string, int>& toVertex, std::vector<std::vector<int>>& adjList, std::vector<std::vector<int>>& invAdjList){
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

void auxTarjan(const int depth, int& maxdepth, const int v, int& index, std::vector<int>& indexes, std::vector<int>& lowlink, std::stack<int>& stack, std::vector<bool>& onStack, int& partition_size, std::vector<int>& partition, std::vector<std::vector<int>>& adjList){
    
    if(depth > maxdepth){
        maxdepth = depth;
    }
    indexes[v] = index;
    lowlink[v] = index;
    index++;
    stack.push(v);
    onStack[v] = true;
    for(const auto w : adjList[v]){
        if(indexes[w] == -1){
            auxTarjan(depth + 1, maxdepth, w, index, indexes, lowlink, stack, onStack, partition_size, partition, adjList);
            lowlink[v] = std::min(lowlink[v], lowlink[w]);
        }
        else if(onStack[w]){
            lowlink[v] = std::min(lowlink[v], indexes[w]);
        }
    }
    if(lowlink[v] == indexes[v]){
        int w;
        do{
            w = stack.top();
            stack.pop();
            onStack[w] = false;
            partition[w] = partition_size;
        }
        while(w != v);
        partition_size++;
    }
}   

std::vector<int> TarjanAlgorithm(std::vector<std::vector<int>>& adjList){
    /*
    isolate every maximal strongly connected component in the graph

    so if C1 -> C2 but not C2 -> C1, we delete every arrow from C1 to C2 and that for every maximal strongly connected component of the graph
    */
    int n = adjList.size();
    int index = 0;
    int partition_size = 0;
    int maxdepth = 0;
    std::vector<int>partition(n, -1);
    std::vector<int>indexes(n, -1);
    std::vector<int>lowlink(n, -1);
    std::vector<bool>onStack(n, false);
    std::stack<int>stack;
    for(int v = 0; v < n; v++){
        if(indexes[v] == -1){
            auxTarjan(0, maxdepth, v, index, indexes, lowlink, stack, onStack, partition_size, partition, adjList);
        }
    }
    std::cout << "number of parts  = " << partition_size << std::endl;
    std::cout << "max depth in dfs = " << maxdepth << std::endl;
    return partition;
}

void applyTarjan(std::vector<std::vector<int>>& adjList, std::vector<std::vector<int>>& invAdjList){
    std::vector<int>partition = TarjanAlgorithm(adjList);
    for(int v = 0; v < adjList.size(); v++){
        std::vector<int>toRemove = {};
        for(const auto w : adjList[v]){
            if(partition[v] != partition[w]){
                // We remember that we must remove w from the out-neighbors of v
                toRemove.push_back(w);
                // we remove v from the in-neighbours of w
                auto it = std::find(invAdjList[w].begin(), invAdjList[w].end(), v);
                if(it != invAdjList[w].end()){
                    invAdjList[w].erase(it);
                }
            }
        }
        // then we remove ever unnecessary out-neighbor of v
        for(const auto w : toRemove){
            // we remove w from the out-neighbours of v
            auto it = std::find(adjList[v].begin(), adjList[v].end(), w);
            if(it != adjList[v].end()){
                adjList[v].erase(it);
            }
        }
    }
}

void statsTarjan(std::vector<std::vector<int>>& adjList, std::vector<std::vector<int>>& invAdjList){
    const int n = adjList.size();
    std::vector<int>partition = TarjanAlgorithm(adjList);
    std::vector<std::vector<int>>invPart(n, std::vector<int>());
    for(int i = 0; i < n; i++){
        if(adjList[i].size() != 0){
            invPart[partition[i]].push_back(i);
        }
    }
    int cpt1 = 0;
    for(int i = 0; i < n; i++){
        if(invPart[i].size() > 0){
            if(invPart[i].size() > 1){
                std::cout << "part " << i << " has " << invPart[i].size() << " elements " << std::endl;
            }
            else{
                cpt1++;
            }
        }
    }
    std::cout << "and " << cpt1 << " CC of size 1" << std::endl;
}





void print_factor(const std::string& s, std::unordered_map<std::string, int>& toVertex, const std::vector<std::vector<int>>& adjList, const int LARGE_WORDS_LENGTH){
    for(int i = 0; i < s.size() - LARGE_WORDS_LENGTH; i = i + FAC_SIZE){
        std::string loc = "";
        for(int j = 0; j < LARGE_WORDS_LENGTH; j++){
            loc = loc + s[i + j];
        }
        std::cout << "(" << toVertex[loc] << " | " << adjList[toVertex[loc]].size() << "°)  -   "; 
    }
    std::cout << std::endl;
}

int right_extends_with_graph_BigStepRauzy(const int factor_id, const std::string& factor, std::unordered_map<std::string, int>& toVertex, std::vector<std::vector<int>>& adjList, std::vector<std::string>& suffix, const int TO , const int LARGE_WORDS_LENGTH){
    std::cout << "try right extend " << factor_id << std::endl;
    int TO_cpt = 0;
    // string for the search with factor as a prefix
    std::string s = factor;
    std::deque<int>path = {factor_id};
    // stack for the backtrack
    std::stack<int> stack;
    for(const int nei : adjList[factor_id]){
        stack.push(nei);
    }
    int longest = 0;
    // For LARGE_WORDS_LENGTH >= 240 !!
    std::vector<std::pair<int, int>>ForcedPositionsP = {std::make_pair<int, int>(20, 8), std::make_pair<int, int>(20, 32), std::make_pair<int, int>(60, 32), std::make_pair<int, int>(120, 8), std::make_pair<int, int>(120, 32)};
    std::vector<std::pair<int, int>>ForcedPositionsQ = {};
    while(!stack.empty() && (TO == -1 || TO_cpt < TO)){
        if(s.size() > longest){
            longest = s.size();
        }
        TO_cpt++;
        // If we want to backtrack
        if(stack.top() == -1){
            stack.pop();
            path.pop_back();
            // we erase the FAC_SIZE last letters of s
            s.erase(s.size() - FAC_SIZE , FAC_SIZE);
        }
        else{
            int local_factor_id = stack.top();
            stack.pop();
            s = s + suffix[local_factor_id];
            path.push_back(local_factor_id);
            // Now, we check that s is a good word
            if(
                checkSquareFreeAtEndV2SuffixString(s, P, FAC_SIZE, true) && 
                //checkSquareFreeForcedPositionsEnd(s, P, FAC_SIZE, ForcedPositionsP) &&
                //checkSquareFreeAtEndV2SuffixString(s, Q, FAC_SIZE, true) && 
                //checkSquareFreeAtEndVector(path, true) &&
                true
            ){ 
                if(s.size() >= SIZE){
                    std::cout << " a large word has been found  : \n" << s << std::endl;
                    return -1;
                }
                stack.push(-1);
                // we compute the current state in the Rauzy graph (maybe can be done better with a stack of states ?)
                std::string current_factor = "";
                for(int i = 0; i < LARGE_WORDS_LENGTH; i++){
                    current_factor = current_factor + s[s.size() - LARGE_WORDS_LENGTH + i];
                }
                for(const int nei : adjList[toVertex[current_factor]]){
                    stack.push(nei);
                }
            }
            else{
                s.erase(s.size() - FAC_SIZE , FAC_SIZE);
                path.pop_back();
            }
        }
    }
    histo_length_right[longest]++;
    return TO_cpt;
}

int left_extends_with_graph_BigStepRauzy(const int factor_id, const std::string& factor, std::unordered_map<std::string, int>& toVertex, std::vector<std::vector<int>>& invAdjList, std::vector<std::string>& prefix, const int TO , const int LARGE_WORDS_LENGTH){
    std::cout << "try left extend " << factor_id << std::endl;
    int TO_cpt = 0;
    // string for the search with factor as a prefix
    std::string s = factor;
    std::deque<int>path = {factor_id};
    // stack for the backtrack
    std::stack<int> stack;
    for(const int nei : invAdjList[factor_id]){
        stack.push(nei);
    }
    int longest = 0;
    // For LARGE_WORDS_LENGTH >= 240 !!
    std::vector<std::pair<int, int>>ForcedPositionsP = {std::make_pair<int, int>(20, 0), std::make_pair<int, int>(20, 32), std::make_pair<int, int>(60, 32), std::make_pair<int, int>(120, 32)};
    std::vector<std::pair<int, int>>ForcedPositionsQ = {};
    while(!stack.empty() && (TO == -1 || TO_cpt < TO)){
        if(s.size() > longest){
            longest = s.size();
        }
        TO_cpt++;
        // If we want to backtrack
        if(stack.top() == -1){
            stack.pop();
            // we erase the FAC_SIZE last letters of s
            s.erase(0 , FAC_SIZE); // erase the prefix of length FAC_SIZE
            path.pop_front();
        }
        else{
            int local_factor_id = stack.top();
            stack.pop();
            s = prefix[local_factor_id] + s;
            path.push_front(local_factor_id);
            // Now, we check that s is a good word
            if(
                checkSquareFreeAtBegV2SuffixString(s, P, FAC_SIZE, true) && 
                //checkSquareFreeForcedPositionsBeg(s, P, FAC_SIZE, ForcedPositionsP) &&
                //checkSquareFreeAtBegV2SuffixString(s, Q, FAC_SIZE, true) &&
                //checkSquareFreeAtBegVector(path, true) &&
                true
            ){ 
                if(s.size() >= SIZE){
                    std::cout << " a large word has been found  : \n" << s << std::endl;
                    return -1;
                }
                stack.push(-1);
                // we compute the current state in the Rauzy graph (maybe can be done better with a stack of states ?)
                std::string current_factor = "";
                for(int i = 0; i < LARGE_WORDS_LENGTH; i++){
                    current_factor = current_factor + s[i];
                }
                for(const int nei : invAdjList[toVertex[current_factor]]){
                    stack.push(nei);
                }
            }
            else{
                s.erase(0 , FAC_SIZE); // erase the prefix of length FAC_SIZE
                path.pop_front();
            }
        }
    }
    histo_length_left[longest]++;
    return TO_cpt;
}
std::vector<int> already_eliminated = {     202290,  // List of prefixes eliminated in a previous computation
                                            197190,
                                            330216,
                                            692226,
                                            524130,
                                            586272,
                                            458412,
                                            118806,
                                            715674,
                                            107538,
                                            10332,
                                            135654,
                                            180024,
                                            406788,
                                            579642,
                                            31038,
                                            136362,
                                            281706,
                                            592014,
                                            213108,
                                            305598,
                                            279168,
                                            67794,
                                            374334,
                                            433740,
                                            76104,
                                            318084,
                                            679506,
                                            155316,
                                            93366,
                                            184908,
                                            224586,
                                            558294,
                                            113814,
                                            731550,
                                            189522,
                                            239340,
                                            738624,
                                            507264,
                                            126156,
                                            691266,
                                            280950,
                                            150924,
                                            685776,
                                            293346,
                                            346716,
                                            73362,
                                            157692,
                                            198660,
                                            403290,
                                            87618,
                                            435486,
                                            448734,
                                            193092,
                                            650766,
                                            177972,
                                            310362,
                                            597552,
                                            325992,
                                            61200,
                                            60654,
                                            350412,
                                            167292,
                                            422172,
                                            75972,
                                            491748,
                                            649920,
                                            169014,
                                            458220,
                                            623184,
                                            37254,
                                            582726,
                                            14664,
                                            532494,
                                            312396,
                                            674304,
                                            704034,
                                            773250,
                                            716502,
                                            519462,
                                            539052,
                                            454554,
                                            323376,
                                            434370,
                                            383394,
                                            713736,
                                            282924,
                                            174882,
                                            409230,
                                            103758,
                                            618708,
                                            761712,
                                            116778,
                                            141858,
                                            667044,
                                            447672,
                                            301482,
                                            406488,
                                            309234,
                                            452106,
                                            268158,
                                            22968,
                                            24834,
                                            259968,
                                            328308,
                                            624726,
                                            46152,
                                            88980,
                                            326130,
                                            159312,
                                            13452,
                                            334884,
                                            77928,
                                            505746,
                                            359532,
                                            555372,
                                            605598,
                                            495708,
                                            359376,
                                            532692,
                                            526026,
                                            693258,
                                            423432,
                                            731460,
                                            523230,
                                            491520,
                                            31668,
                                            613764,
                                            65670,
                                            422412,
                                            354384,
                                            277644,
                                            239412,
                                            386352,
                                            629436,
                                            132924,
                                            215412,
                                            3876,
                                            141210,
                                            294090,
                                            651216,
                                            241986,
                                            192270,
                                            434700,
                                            434406,
                                            186144,
                                            769326,
                                            33642,
                                            375732,
                                            593850,
                                            397920,
                                            134766,
                                            672174,
                                            46476,
                                            277206,
                                            251736,
                                            135948,
                                            517356,
                                            321570,
                                            466020,
                                            127068,
                                            334248,
                                            569718,
                                            755256,
                                            270492,
                                            67398,
                                            451350,
                                            168114,
                                            709686,
                                            105384,
                                            615630,
                                            646848,
                                            72048,
                                            521334,
                                            270594,
                                            167256,
                                            363528,
                                            741078,
                                            502422,
                                            238272,
                                            422364,
                                            25620,
                                            132732,
                                            759756,
                                            769158,
                                            753792,
                                            396264,
                                            325656,
                                            65478,
                                            684438,
                                            684162,
                                            313380,
                                            366174,
                                            203922,
                                            299022,
                                            267516,
                                            312078,
                                            134022,
                                            528702,
                                            475284,
                                            309810,
                                            210474,
                                            235446,
                                            57234,
                                            540030,
                                            567918,
                                            723360,
                                            591324,
                                            397128,
                                            263604,
                                            15738,
                                            308466,
                                            202710,
                                            382782,
                                            296496,
                                            229650,
                                            691788,
                                            558930,
                                            71280,
                                            753282,
                                            777072,
                                            139380,
                                            89346,
                                            352140,
                                            709452,
                                            12348,
                                            96222,
                                            605370,
                                            171876,
                                            268920,
                                            35616,
                                            44118,
                                            529206,
                                            246198,
                                            599874,
                                            479670,
                                            310668,
                                            96192,
                                            3576,
                                            550932,
                                            242868,
                                            178452,
                                            134622,
                                            91836,
                                            443112,
                                            93216,
                                            24774,
                                            365964,
                                            377532,
                                            670116,
                                            759540,
                                            93300,
                                            127302,
                                            340842,
                                            256782,
                                            323640,
                                            534870,
                                            468054,
                                            406590,
                                            595242,
                                            384000,
                                            342246,
                                            417126,
                                            665172,
                                            251538,
                                            123318,
                                            124680,
                                            320208,
                                            319098,
                                            761808,
                                            130956,
                                            732378,
                                            663144,
                                            232032,
                                            150438,
                                            276846,
                                            378360,
                                            472326,
                                            258414,
                                            653256,
                                            413844,
                                            638592,
                                            400896,
                                            315042,
                                            102186,
                                            478596,
                                            196848,
                                            148524,
                                            651276,
                                            696822,
                                            560310,
                                            740142,
                                            673050,
                                            267114,
                                            146118,
                                            578748,
                                            636972,
                                            31716,
                                            488460,
                                            702558,
                                            110418,
                                            576480,
                                            319128,
                                            99570,
                                            492222,
                                            233526,
                                            152844,
                                            571974,
                                            131712,
                                            200598,
                                            455676,
                                            593922,
                                            365850,
                                            126396,
                                            7038,
                                            174366,
                                            596220,
                                            609810,
                                            700374,
                                            516366,
                                            345924,
                                            457644,
                                            234096,
                                            622530,
                                            62304,
                                            248736,
                                            387774,
                                            116634,
                                            727158,
                                            392832,
                                            46104,
                                            358284,
                                            231810,
                                            625050,
                                            663222,
                                            434814,
                                            500430,
                                            589698,
                                            755052,
                                            173874,
                                            13926,
                                            481650,
                                            21828,
                                            218532,
                                            705672,
                                            146130,
                                            717684,
                                            223176,
                                            472386,
                                            304650,
                                            460008,
                                            694176,
                                            229944,
                                            453480,
                                            603888,
                                            712014,
                                            14070,
                                            468792,
                                            72114,
                                            162984,
                                            672858,
                                            337374,
                                            52554,
                                            506712,
                                            231426,
                                            419652,
                                            440298,
                                            738480,
                                            632250,
                                            192600,
                                            710136,
                                            651162,
                                            243024,
                                            353238,
                                            280776,
                                            612018,
                                            572022,
                                            502074,
                                            441600,
                                            81864,
                                            556362,
                                            28032,
                                            131316,
                                            264876,
                                            31608,
                                            515016,
                                            182256,
                                            31290,
                                            480408,
                                            599364,
                                            339234,
                                            111870,
                                            295920,
                                            514596,
                                            294030,
                                            624846,
                                            279540,
                                            515550,
                                            389778,
                                            15252,
                                            745926,
                                            353616,
                                            26190,
                                            753408,
                                            39678,
                                            775638,
                                            473700,
                                            296790,
                                            738432,
                                            623676,
                                            145458,
                                            484710,
                                            428316,
                                            464022,
                                            274764,
                                            600018,
                                            591174,
                                            278508,
                                            217956,
                                            633354,
                                            686598,
                                            725784,
                                            688008,
                                            664086,
                                            162360,
                                            728190,
                                            327192,
                                            2460,
                                            674154,
                                            431604,
                                            74850,
                                            6834,
                                            20952,
                                            322650,
                                            37536,
                                            420588,
                                            51306,
                                            744360,
                                            672996,
                                            17244,
                                            653022,
                                            470742,
                                            128460,
                                            682032,
                                            579570,
                                            139602,
                                            373344,
                                            107232,
                                            118770,
                                            117390,
                                            246108,
                                            243234,
                                            308904,
                                            681642,
                                            186654,
                                            263412,
                                            32352,
                                            256962,
                                            435402,
                                            752370,
                                            688482,
                                            174246,
                                            593214,
                                            215010,
                                            587346,
                                            651222,
                                            132540,
                                            444600,
                                            207258,
                                            221148,
                                            46440,
                                            642546,
                                            287340,
                                            311646,
                                            406242,
                                            544932,
                                            618066,
                                            603690,
                                            271146,
                                            176490,
                                            411258,
                                            24480,
                                            246120,
                                            312432,
                                            441504,
                                            698376,
                                            34308,
                                            337248,
                                            762102,
                                            464310,
                                            173964,
                                            554604,
                                            529980,
                                            58920,
                                            120888,
                                            553740,
                                            365784,
                                            358710,
                                            708018,
                                            257292,
                                            470088,
                                            765222,
                                            277986,
                                            671820,
                                            117036,
                                            660744,
                                            77226,
                                            172056,
                                            700854,
                                            631044,
                                            167160,
                                            561966,
                                            40584,
                                            204096,
                                            571404,
                                            529476,
                                            739218,
                                            524952,
                                            161220,
                                            116406,
                                            179502,
                                            391032,
                                            377106,
                                            74424,
                                            387138,
                                            32070,
                                            135174,
                                            362946,
                                            333594,
                                            380418,
                                            99930,
                                            300954,
                                            175536,
                                            218670,
                                            22620,
                                            674052,
                                            677400,
                                            75420,
                                            501948,
                                            218910,
                                            651108,
                                            343734,
                                            463728,
                                            274914,
                                            236256,
                                            244938,
                                            398502,
                                            502968,
                                            726798,
                                            720888,
                                            165204,
                                            362148,
                                            250026,
                                            460050,
                                            269874,
                                            669264,
                                            430710,
                                            11550,
                                            114744,
                                            549810,
                                            47190,
                                            30090,
                                            182754,
                                            186684,
                                            558426,
                                            158556,
                                            594858,
                                            292926,
                                            104208,
                                            541350,
                                            568560,
                                            190890,
                                            762498,
                                            184284,
                                            407592,
                                            589806,
                                            569670,
                                            660240,
                                            779286,
                                            665844,
                                            431928,
                                            628626,
                                            725082,
                                            343728,
                                            625716,
                                            684594,
                                            619842,
                                            491664,
                                            611472,
                                            142596,
                                            173622,
                                            426918,
                                            464724,
                                            8580,
                                            422760,
                                            511218,
                                            32448,
                                            86928,
                                            65172,
                                            378066,
                                            3972,
                                            114798,
                                            773928,
                                            691938,
                                            142404,
                                            533232,
                                            572448,
                                            691806,
                                            51636,
                                            71880,
                                            25074,
                                            490080,
                                            439812,
                                            144930,
                                            599682,
                                            55998,
                                            542298,
                                            417474,
                                            448404,
                                            297696,
                                            337692,
                                            213618,
                                            33960,
                                            315402,
                                            330978,
                                            58158,
                                            215904,
                                            37962,
                                            469164,
                                            456162,
                                            210750,
                                            249108,
                                            626538,
                                            246918,
                                            428106,
                                            485658,
                                            121752,
                                            279750,
                                            54228,
                                            216774,
                                            23490,
                                            218736,
                                            90594,
                                            483474,
                                            459726,
                                            696576,
                                            729738,
                                            316254,
                                            624240,
                                            109764,
                                            95220,
                                            100866,
                                            723984,
                                            140340,
                                            331590,
                                            539430,
                                            137172,
                                            244218,
                                            562872,
                                            17286,
                                            31014,
                                            318696,
                                            711456,
                                            695748,
                                            521232,
                                            82812,
                                            82752,
                                            496626,
                                            192138,
                                            315438,
                                            353442,
                                            652992,
                                            108864,
                                            209712,
                                            163962,
                                            265956,
                                            392436,
                                            622158,
                                            17580,
                                            569352,
                                            271326,
                                            733020,
                                            316896,
                                            437910,
                                            500574,
                                            704574,
                                            51624,
                                            244932,
                                            27486,
                                            283596,
                                            721554,
                                            66042,
                                            509622,
                                            646164,
                                            322632,
                                            485262,
                                            261174,
                                            113346,
                                            196740,
                                            466860,
                                            477978,
                                            709632,
                                            203424,
                                            24324,
                                            154770,
                                            452460,
                                            390666,
                                            11562,
                                            323196,
                                            708336,
                                            586812,
                                            482250,
                                            68376,
                                            179718,
                                            717594,
                                            774006,
                                            440862,
                                            142896,
                                            536076,
                                            553878,
                                            371202,
                                            774936,
                                            57984,
                                            50610,
                                            718050,
                                            77130,
                                            683742,
                                            25662,
                                            621168,
                                            212154,
                                            580134,
                                            524190,
                                            94020,
                                            564438,
                                            188616,
                                            437940,
                                            475524,
                                            262086,
                                            297426,
                                            501174,
                                            88962,
                                            581640,
                                            172932,
                                            496404,
                                            466314,
                                            689640,
                                            591852,
                                            94974,
                                            572382,
                                            15942,
                                            408318,
                                            427458,
                                            187068,
                                            559920,
                                            592698,
                                            572658,
                                            160368,
                                            529728,
                                            503922,
                                            254184,
                                            322830,
                                            130992,
                                            66060,
                                            675144,
                                            49818,
                                            153588,
                                            125736,
                                            534228,
                                            62820,
                                            544044,
                                            618942,
                                            405594,
                                            647046,
                                            139716,
                                            723564,
                                            246366,
                                            772506,
                                            345492,
                                            431940,
                                            168468,
                                            304350,
                                            648036,
                                            675390,
                                            269286,
                                            378120,
                                            167748,
                                            91200,
                                            377718,
                                            289254,
                                            647808,
                                            62592,
                                            266382,
                                            410550,
                                            437346,
                                            486138,
                                            58884,
                                            691692,
                                            556032,
                                            288180,
                                            180660,
                                            351720,
                                            171462,
                                            552954,
                                            223260,
                                            559284,
                                            676674,
                                            335430,
                                            523362,
                                            663840,
                                            102864,
                                            473814,
                                            151716,
                                            616200,
                                            637446,
                                            80268,
                                            418884,
                                            547926,
                                            254826,
                                            367770,
                                            344586,
                                            686454,
                                            747684,
                                            54552,
                                            69534,
                                            255606,
                                            218808,
                                            365058,
                                            774318,
                                            757122,
                                            721092,
                                            316524,
                                            104718,
                                            79218,
                                            536166,
                                            244512,
                                            165744,
                                            11004,
                                            317928,
                                            470196,
                                            440790,
                                            12792,
                                            223968,
                                            659448,
                                            154140,
                                            562056,
                                            475530,
                                            180720,
                                            738486,
                                            465234,
                                            568794,
                                            448044,
                                            192882,
                                            595434,
                                            646332,
                                            664062,
                                            126618,
                                            288414,
                                            25122,
                                            670074,
                                            698364,
                                            322560,
                                            217242,
                                            82152,
                                            205038,
                                            76476,
                                            446568,
                                            136734,
                                            342402,
                                            709554,
                                            592368,
                                            191826,
                                            236814,
                                            652458,
                                            81894,
                                            464574,
                                            756126,
                                            90432,
                                            427674,
                                            455274,
                                            533292,
                                            22380,
                                            459360,
                                            772068,
                                            773130,
                                            640152,
                                            545874,
                                            74088,
                                            76122,
                                            675648,
                                            230916,
                                            493314,
                                            4350,
                                            423318,
                                            143028,
                                            210000,
                                            297438,
                                            341256,
                                            709662,
                                            352266,
                                            36900,
                                            22314,
                                            360738,
                                            182148,
                                            85608,
                                            583512,
                                            54306,
                                            457446,
                                            17280,
                                            130494,
                                            354684,
                                            741912,
                                            580884,
                                            751188,
                                            562296,
                                            637824,
                                            283080,
                                            481308,
                                            8574,
                                            289350,
                                            324414,
                                            732324,
                                            408870,
                                            139464,
                                            585282,
                                            167208,
                                            461526,
                                            739680,
                                            647622,
                                            292008,
                                            729234,
                                            216276,
                                            525978,
                                            181746,
                                            52200,
                                            449148,
                                            482526,
                                            701064,
                                            307224,
                                            525276,
                                            602892,
                                            93516,
                                            628986,
                                            186042,
                                            563562,
                                            208596,
                                            91632,
                                            254400,
                                            555498,
                                            671862,
                                            236676,
                                            292254,
                                            669126,
                                            439068,
                                            185088,
                                            678438,
                                            97704,
                                            389484,
                                            750432,
                                            249384,
                                            779562,
                                            27288};

void searchTimeOut_with_graph_BigStepRauzy(const std::vector<std::string>& factors, std::unordered_map<std::string, int>& toVertex, std::vector<std::vector<int>>& adjList, std::vector<std::vector<int>>& invAdjList, const int LARGE_WORDS_LENGTH){
    /*
    backtrack algorithm using the Rauzy graph in the search

    */
    std::cout << "start search with big step Rauzy graph, LARGE_WORDS_LENGTH = " << LARGE_WORDS_LENGTH << std::endl;
    std::vector<std::string> all_factors = all_words(factors); // notice that all_factors is the inverse unordered_map of toVertex
    for(const auto& factor : all_factors){
        std::cout << factor << " - " << toVertex[factor] << std::endl;
    }
    // We first precompute the suffixes of length FAC_SIZE
    std::vector<std::string> suffix; // stores the suffixs of lengths FAC_SIZE
    std::vector<std::string> prefix; // stores the prefixs of lengths FAC_SIZE
    for(const auto& factor : all_factors){
        std::string su = "";
        std::string pr = "";
        for(int i = 0; i < FAC_SIZE; i++){
            su = su + factor[factor.size() - FAC_SIZE + i];
            pr = pr + factor[i];
        }
        suffix.push_back(su);
        prefix.push_back(pr);
    }
    int startingTO = 2000;
    int TO = startingTO;
    int fp_detector = 0;
    std::deque<int> fifo;
    for(int i = 0; i < factors.size(); i++){
        // If the vertex of ID i has not been eliminated, we add it to FIFO
        if(std::find(already_eliminated.begin(), already_eliminated.end(), 6*i) == already_eliminated.end()){
            fifo.push_back(6*i);
        }
        // Otherwise, we remove it from the graph
        else{
            std::cout << "eliminated : " << i << std::endl;
            std::string factor = all_factors[i];
            for(const auto& factor_to_delete : all_words({factor})){
                for(const auto right_nei : adjList[toVertex[factor_to_delete]]){
                    auto it = std::find(invAdjList[right_nei].begin(), invAdjList[right_nei].end(), toVertex[factor_to_delete]);
                    if(it != invAdjList[right_nei].end()){
                        invAdjList[right_nei].erase(it);
                    }
                }
                adjList[toVertex[factor_to_delete]] = {};
            }
            //toVertex.erase(factor_to_delete);
            prunning_graph_BigStepRauzy(toVertex, adjList, invAdjList);
        }   
    }
    applyTarjan(adjList, invAdjList);
    std::cout << "FIFO is built" << std::endl;
    statistics(toVertex, adjList, invAdjList);
    int cpt_hour = 0;
    while(fifo.size() > 0){
        if(cpt_hour == 15){
            auto end = std::chrono::high_resolution_clock::now();
            auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
            std::cout << "TIME FROM START = " << duration.count()/1000 <<"s" << std::endl;
            cpt_hour = 0;
        }
        else{
            cpt_hour++;
        }
        int factor_id = fifo.front();
        std::string factor = all_factors[factor_id];
        fifo.pop_front();
        int TO_cpt_right;
        int TO_cpt_left;
        if(adjList[factor_id].size() == 0){
            TO_cpt_right = 0;
            TO_cpt_left = 0;
        }
        else{
            std::cout << "|fifo| = " << fifo.size() << " TO = " << TO << " FP = " << fp_detector << std::endl;
            TO_cpt_right = right_extends_with_graph_BigStepRauzy(factor_id, factor, toVertex, adjList, suffix, TO, LARGE_WORDS_LENGTH);
            if(TO_cpt_right >= TO){
                TO_cpt_left = left_extends_with_graph_BigStepRauzy(factor_id, factor, toVertex, invAdjList, prefix, TO, LARGE_WORDS_LENGTH);
            }
           else{
                TO_cpt_left = TO_cpt_right;
           }
           std::cout << " TO left = " << TO_cpt_left << " TO right = " << TO_cpt_right << std::endl;
        }
        // if the backtrack ended before the Time Out
        if(TO_cpt_right == -1 || TO_cpt_left == -1){
            return;
        }
        else if(TO_cpt_right < TO || TO_cpt_left < TO){ // we can eliminate a prefix !
            std::cout << "eliminated : " << toVertex[factor] << std::endl;
            // we reset the counter for detection of fixed point
            fp_detector = 0;
            // we can cut the graph by removing factor (and the equivalent factors)
            for(const auto& factor_to_delete : all_words({factor})){
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
            applyTarjan(adjList, invAdjList);
            statistics(toVertex, adjList, invAdjList);
        }
        // otherwise
        else{
            // we put factor at the end of the deque
            fifo.push_back(factor_id);
        }
        fp_detector++;
        if(fp_detector > fifo.size()/25){
            statistics(toVertex, adjList, invAdjList);
            if(TO == startingTO){
                applyTarjan(adjList, invAdjList);
                TO = 30000000;
                for(int j = 0; j < 4000; j++){
                    histo_length_right[j] = 0;
                    histo_length_left[j] = 0;
                }
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
    // std::vector<std::string> l = search_factors();
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
    std::vector<std::string> aw = all_words(l);
    std::unordered_map<std::string, std::vector<std::string>> R = Rauzy(aw); // Rauzy graph
    std::vector<std::string> lw = long_words(l, R, LARGE_WORDS_LENGTH); 
    std::random_device rd;
    std::mt19937 g(rd());
    std::shuffle(lw.begin(), lw.end(), g);
    std::cout << lw.size() << std::endl;
    std::tuple<std::unordered_map<std::string, int>, std::vector<std::vector<int>>, std::vector<std::vector<int>>> G =  graph_BigStepRauzy(all_words(lw), aw);
    std::unordered_map<std::string, int> toVertex = std::get<0>(G); 
    std::vector<std::vector<int>> adjList = std::get<1>(G); 
    std::vector<std::vector<int>> invAdjList = std::get<2>(G);
    std::cout << " statistics pre prunning : " << std::endl;
    statistics(toVertex, adjList, invAdjList);
    std::cout << "prunning." << std::endl;
    // We recursively disconnect the 0 degree vertices
    prunning_graph_BigStepRauzy(toVertex, adjList, invAdjList);
    std::cout << " statistics post prunning : " << std::endl;
    statistics(toVertex, adjList, invAdjList);
    // // Then we disconnect the strongly connected components
    std::cout << "Tarjan Algorithm." << std::endl;
    statsTarjan(adjList, invAdjList);
    applyTarjan(adjList, invAdjList);
    std::cout << " statistics post Tarjan : " << std::endl;
    statistics(toVertex, adjList, invAdjList);
    std::cout << "re pruning, nothing should change" << std::endl;
    prunning_graph_BigStepRauzy(toVertex, adjList, invAdjList);
    std::cout << " statistics post prunning post Tarjan: " << std::endl;
    statistics(toVertex, adjList, invAdjList);

    searchTimeOut_with_graph_BigStepRauzy(lw, toVertex, adjList, invAdjList, LARGE_WORDS_LENGTH);
    std::cout << "\n---------------------------------------------------------\n# of squares of given period and position : \n for P = " << P << " : \n"<< std::endl;
    for(int period = 1; period < 2000; period++){
        for(int end = 0; end < 2000; end++){
            if(squaresEndP[period][end] != 0 ||  squaresBegP[period][end] != 0){
                std::cout << "period = " << std::setw(5) << period << " end = " << std::setw(5) << end <<
                            " # squares mod P end = " << std::setw(11) << squaresEndP[period][end] << 
                            " # squares mod P beg = " << std::setw(11) << squaresBegP[period][end] <<  std::endl;
            }
        }
    }
    std::cout << "For Q = " << Q << ": \n" << std::endl;
    for(int period = 1; period < 2000; period++){
        for(int end = 0; end < 2000; end++){
            if(squaresEndQ[period][end] != 0 || squaresBegQ[period][end] != 0){
                std::cout << "period = " << std::setw(5) << period << " end = " << std::setw(5) << end <<
                            " # squares mod Q end = " << std::setw(11) << squaresEndQ[period][end] << 
                            " # squares mod Q beg = " << std::setw(11) << squaresBegQ[period][end] <<  std::endl;
            }
        }
    }
    std::cout << "For path : \n" << std::endl;
    for(int period = 1; period < 2000; period++){
        if(squaresEndPath[period] != 0 || squaresBegPath[period] != 0){
            std::cout << "period = " << std::setw(5) << period <<
                        " # squares path end = " << std::setw(11) << squaresEndPath[period] << 
                        " # squares path beg = " << std::setw(11) << squaresBegPath[period] <<  std::endl;
        }
    }
    std::cout << "\n---------------------------------------------------------\n long found words : \n"<< std::endl;
    std::cout << "size        | histo left  |   histo right" << std::endl;
    for(int i = 0; i < 4000; i++){
        if(histo_length_right[i] != 0 || histo_length_left[i] != 0){
            std::cout << std::setw(11) << i << " | " << std::setw(11)  << histo_length_left[i] << " | " << std::setw(11) << histo_length_right[i] << std::endl;
        }
    }
    return 0;
}

