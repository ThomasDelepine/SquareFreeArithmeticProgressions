#!/bin/bash


# compilation
g++ -O3 -march=native -flto -g -o table_D_red.o table_D_red.cpp


# Array of pairs
pairs=(
    "3 4"       # terminates
    "3 5"       # terminates
    "3 7"       # terminates
    "3 8"       # terminates
    "3 10"      # terminates
    "4 5"       # terminates
    "4 7"       # terminates
    "4 9"       # terminates
    "4 10"      # terminates
    #"5 8"       # stuck at about 1250
    "6 7"       # terminates
    #"9 15"      # stuck at about 9500
)

# loop over the pairs
for pair in "${pairs[@]}"; do
	./table_D_red.o $pair
done