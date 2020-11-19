#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

int main(int argc, char* argv[]) {
    FILE* fptr;
    int instr = 0;
    if (argc < 2) printf("Please enter the executable");
    fptr = fopen(argv[1], 'rb');
    if (fptr == 0) printf("Unable to read the executable");
}