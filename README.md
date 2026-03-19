# Pairs of square-free arithmetic progressions in infinite ternary square-free words

This repository provides the C++ implementations used for computer assisted proofs of several lemmas in the associated paper ***arxiv link***.

### changer les titres pour : [Proposition 11](https://arxiv.org/pdf/2602.13037#theorem.11)

### Lemma 8
- [lemma_8.cpp](https://github.com/ThomasDelepine/SquareFreeArithmeticProgressions/blob/main/lemma_8.cpp): Code that verifies that
    - the patterns $ab$ are $(12, h_{26})$-recurrent for $a \neq b$, and that
	- the patterns $a⋄^db$ are $(27, h_{26})$-recurrent for every $d$ in $\{1, 2, 4, 5, 6, 7, 8\}$
    ```sh
    g++ -O3 -o lemma_8.o lemma_8.cpp
    ./lemma_8.o
    ```
### Lemma 10
- [lemma_10.cpp](https://github.com/ThomasDelepine/SquareFreeArithmeticProgressions/blob/main/lemma_F.cpp): Code that verifies that 
    - the pattern $a⋄^3b$ is $(10, h)$-constructible, and that 
    - the patterns $a⋄^db$ are $(8, h)$-constructible for every $d$ in $\{9, ..., 15\}$
    ```sh
    g++ -O3 -o lemma_10.o lemma_10.cpp
    ./lemma_10.o
    ```
### Lemma 13
- [lemma_13_i.cpp](https://github.com/ThomasDelepine/SquareFreeArithmeticProgressions/blob/main/lemma_B_1.cpp): Code that verifies that the patterns $\bar{a}⋄^d\bar{b}$ are $(6, h_{26})$-recurrent for every $d$ in $\{0..17\}$
    ```sh
    g++ -O3 -o lemma_13_i.o lemma_13_i.cpp
    ./lemma_13_i.o
    ```

- [lemma_13_ii.cpp](https://github.com/ThomasDelepine/SquareFreeArithmeticProgressions/blob/main/lemma_B_2.cpp): Code that verifies that the patterns $\mathcal{P} = \bar{a}⋄^db⋄^d\bar{c}$ are $(27, h_{26})$-recurrent for every $d$ in $\{0..17\}$ and $\mathcal{P}$ not in $\mathcal{P}_{bad}$ 
    ```sh
    g++ -O3 -o lemma_13_ii.o lemma_13_ii.cpp
    ./lemma_13_ii.o
    ```

- [lemma_14.cpp](https://github.com/ThomasDelepine/SquareFreeArithmeticProgressions/blob/main/lemma_C.cpp): Code that verifies that the patterns from $\mathcal{P}_{bad}$ are $(13, h)$-constructible
    ```sh
    g++ -O3 -o lemma_14.o lemma_14.cpp
    ./lemma_14.o
    ```
### Table 1
- [table_1_blue.cpp](https://github.com/ThomasDelepine/SquareFreeArithmeticProgressions/blob/main/table_D_blue.cpp) Code that verifies that the pair $(p, q)$ is a blue pair from [Table 1](Mettre lien) are positive pairs
    ```sh
    g++ -O3 -o table_1_blue.o table_1_blue.cpp
    ./table_1_blue.o <p> <q>
    ```

- [table_1_red.cpp](https://github.com/ThomasDelepine/SquareFreeArithmeticProgressions/blob/main/table_D_red.cpp) Code that verifies that the pair $(p, q)$ is a red pair from [Table 1](Mettre lien) are negative pairs
    ```sh
    g++ -O3 -o table_1_red.o table_1_red.cpp
    ./table_1_red.o <p> <q>
    ```
    To run all tests for [Table 1](Mettre lien) : 
    ```sh 
    bash table_1.sh
    ```
    
### Table 2
- [table_2.cpp](https://github.com/ThomasDelepine/SquareFreeArithmeticProgressions/blob/main/table_D_blue.cpp) Code that verifies that there exists a morphism that is square-free and whose subsequences congruent to $\alpha$ modulo $p$ are also square-free
    ```sh
    g++ -O3 -o table_2.o table_2.cpp
    ./table_2.o <p> 
    ```
    To run all tests for [Table 2](Mettre lien) : 
    
    ```sh 
    bash table_2.sh
    ```
    
### Counting the uncovered pairs 
Code that counts the number of pairs of coprime integers greater than or equal to 3 that are not covered
- [filtre.py](https://github.com/ThomasDelepine/SquareFreeArithmeticProgressions/blob/main/filtre.py)
    ```sh
    python3 filtre.py
    ```