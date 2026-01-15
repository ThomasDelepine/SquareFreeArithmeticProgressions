#!/bin/bash


# compilation
g++ -O3 -o table_D_red.o table_D_red.cpp
g++ -O3 -o table_D_blue.o table_D_blue.cpp
##gcc -O3 -o table_D_blue.o table_D_blue.c


# Array of pairs
pairs=(
    "3 4"       
    "3 5" 
    "3 6"      
    "3 7"       
    "3 8"       
    "3 10"      
    "4 5" 
    "4 6"      
    "4 7"  
    "4 8"     
    "4 9"       
    "4 10"      
    "4 14"  
    "5 10"    
    "6 7" # takes a few seconds but terminates
    "6 9"
    "6 12" 
    "7 14"
    "8 12"
    "8 16"
    "9 18"
    "10 15"
    "10 20"
    "12 18"     
)

for ((i=3; i<=19; i++)); do
  for ((j=i+1; j<=20; j++)); do
    if (( i != 5 || j != 8 )); then # We do not consider the pair (5, 8)
      if printf '%s\n' "${pairs[@]}" | grep -qx "$i $j"; then
        ./table_D_red.o "$i" "$j"
      else
        ./table_D_blue.o "$i" "$j"
      fi
    fi
  done
done
