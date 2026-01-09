#!/bin/bash


# compilation
g++ -O3 -march=native -flto -g -o table_E.o table_E.cpp


# Array of integers to check
integers=(
    "3"
    "4"
    "5"
    "7"
    "8"
    "9"
    "10"
    "11"
    "12"
    "14"
    "15"
    "16"
    "20"
    "21"
    "22"
)

for i in "${integers[@]}"; do
    ./table_E.o "$i"
done
