#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

struct internal_state {
int register_a = 0, register_b = 0, stack_pointer = 0, program_counter = 0;
};
void fetch (FILE** fin) {

}
void decode_and_execute(int instruction) {

}
int main(int argc, char* argv[]) {
    FILE* fptr;
    int instr = 0;
    if (argc < 2) printf("Please enter the executable");
    fptr = fopen(argv[1], 'rb');
    if (fptr == 0) printf("Unable to read the executable");
    while(true) {
        int instruction  = fetch(fin);
        decode_and_execute(instruction);
        if (feof(fptr)) break;
    }
}