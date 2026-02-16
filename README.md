# Pairs of square-free arithmetic progressions in infinite ternary square-free words

This repository provides the C++ implementations used in the proofs of several lemmas in the associated paper ***arxiv link***.

***recheck Lemma names***
***update the links***

### changer les titres pour : [Proposition 11](https://arxiv.org/pdf/2602.13037#theorem.11)

### Lemma 8
- [lemma_8.cpp](https://github.com/ThomasDelepine/SquareFreeArithmeticProgressions/blob/main/lemma_A.cpp): Code that verifies that
    - the patterns ab are (12, h26)-recurrent for a != b, and that
	- the patterns (a)⋄^d^(b) are (27, h26)-recurrent for any d in {1, 2, 4, 5, 6, 7, 8}
    ```sh
    g++ -O3 -o lemma_8.o lemma_8.cpp
    ./lemma_8.o
    ```
### Lemma 10
- [lemma_10.cpp](https://github.com/ThomasDelepine/SquareFreeArithmeticProgressions/blob/main/lemma_F.cpp): Code that verifies that 
    - the pattern (a)⋄^3^(b) is (10, h)-constructible, and that 
    - the patterns (a)⋄^d^(b) are (8, h)-constructible for every d in {9, ..., 15}
    ```sh
    g++ -O3 -o lemma_10.o lemma_10.cpp
    ./lemma_10.o
    ```
### Lemma 13
- [lemma_13_i.cpp](https://github.com/ThomasDelepine/SquareFreeArithmeticProgressions/blob/main/lemma_B_1.cpp): Code that verifies that the patterns (!a)⋄^d^(!b) are (6, h)-recurrent for d in {0..17}
    ```sh
    g++ -O3 -o lemma_13_i.o lemma_13_i.cpp
    ./lemma_13_i.o
    ```

- [lemma_13_ii.cpp](https://github.com/ThomasDelepine/SquareFreeArithmeticProgressions/blob/main/lemma_B_2.cpp): Code that verifies that the patterns P = (!a)⋄^d^(b)⋄^d^(!c) are (27, h)-recurrent for d in {0..17} and P not in Pbad 
    ```sh
    g++ -O3 -o lemma_13_ii.o lemma_13_ii.cpp
    ./lemma_13_ii.o
    ```

- [lemma_14.cpp](https://github.com/ThomasDelepine/SquareFreeArithmeticProgressions/blob/main/lemma_C.cpp): Code that verifies that the patterns from PBad are (13, h)-constructible
    ```sh
    g++ -O3 -o lemma_14.o lemma_14.cpp
    ./lemma_14.o
    ```
### Table 1
- [table_1_blue.cpp](https://github.com/ThomasDelepine/SquareFreeArithmeticProgressions/blob/main/table_D_blue.cpp) Code that verifies that the blue pairs from [Table 1](Mettre lien) are positive solutions
    ```sh
    g++ -O3 -o table_1_blue.o table_1_blue.cpp
    ./table_1_blue.o <p> <q>
    ```

- [table_1_red.cpp](https://github.com/ThomasDelepine/SquareFreeArithmeticProgressions/blob/main/table_D_red.cpp) Code that verifies that the red pairs from [Table 1](Mettre lien) are negative solutions
    ```sh
    g++ -O3 -o table_1_red.o table_1_red.cpp
    ./table_1_red.o <p> <q>
    ```
    To run all tests for [Table 1](Mettre lien) : ```bash table_1.sh```
    
### Table 2
- [table_2.cpp](https://github.com/ThomasDelepine/SquareFreeArithmeticProgressions/blob/main/table_D_blue.cpp) Code that verifies that there exists a morphism that is square-free and whose subsequences congruent to alpha modulo p are also square-free
    ```sh
    g++ -O3 -o table_2.o table_2.cpp
    ./table_2.o <p> 
    ```
    To run all tests for [Table 2](Mettre lien) :   ```bash table_2.sh```
    
### Counting the uncovered pairs Code that counts the number of pairs of coprime integers greater than or equal to 3 that are not covered by the results of ...
- [filtre.py](https://github.com/ThomasDelepine/SquareFreeArithmeticProgressions/blob/main/filtre.py)
    ```sh
    python3 filtre.py
    ```