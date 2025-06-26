#!/bin/bash


# compilation
g++ -O3 -march=native -flto -g -o table_D_red.o table_D_red.cpp


# Array of pairs
pairs=(
    "3 4"
    "3 5"
    "3 7"
    "3 8"
    "3 10"
    "4 5"
    "4 7"
    "4 9"
    "4 10"
    "5 8"
    "6 7"
    "9 15"
)

# loop over the pairs
for pair in "${pairs[@]}"; do
	./table_D_red.o $pair
done