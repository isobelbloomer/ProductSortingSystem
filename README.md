# ProductSortingSystem - C

This is a **C** program that reads in data from text files and uses circular queues to merge the data into one list sorted by order of weight. This program then allows the user to search for an item by weight.

This project received **100%** in Algorithm Design at TU Dublin.

## How it works
- The program reads in lists of data from 4 text files.
- It uses circular queues to sort data from 4 seperate files into 1 list, ordered by weight.
- All functionality is implemented in a single file (`main.c`).

## Files
- src/main.c → main program
- data/line1.txt, data/line2.txt, data/line3.txt, data/line4.txt → sample input files
- Makefile → build configuration

## Build
```bash
make

## Run
./main

## Clean
to remove the compiled executable
make clean

