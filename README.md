# ProductSortingSystem - C

This is a **C** program that reads in data from text files and uses circular queues to merge the data into one list sorted by order of weight. This program then allows the user to search for an item by weight.

This project received **100%** in Algorithm Design at TU Dublin.

## How it works
- The program reads in lists of data from 4 text files.
- It uses circular queues to sort data from 4 seperate files into 1 list, ordered by weight.
- All functionality is implemented in a single file (`main.c`).

## Files
- `Main.c` → main program
- `line1.txt`, `line2.txt`, `line3.txt`, `line4.txt` → sample input files

## How to run
gcc Main.c -o Main
./Main

