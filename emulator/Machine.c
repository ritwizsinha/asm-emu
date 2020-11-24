#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#define memory_size 1000

int exit_loop = 0;
int memory_store[memory_size];
int size = 0;

/* Internal State */
int register_a = 0 ,register_b = 0, stack_pointer = 0, program_counter = 0;
void ldc(int val) {
    register_b = register_a;
    register_a = val;
}

void adc(int val) {
    register_a+=val;
}

void ldl(int val) {
    register_b = register_a;
    register_a = stack_pointer + val >= size ? 0 : memory_store[stack_pointer + val];
}

void stl (int val) {
    if (stack_pointer + val < size) {
        memory_store[stack_pointer + val] = register_a;
        register_a = register_b;
    }
} 

void ldnl(int val) {
    register_a = register_a + val >= size ? 0 : memory_store[register_a + val];
}

void stnl(int val) {
    if (register_a + val< size) memory_store[register_a + val]  = register_b;
}

void add(int val) {
    register_a +=register_b;
}

void sub(int val) {
    register_a = register_b - register_a;
}

void shl(int val) {
    register_a = register_b<<register_a;
}

void shr(int val) {
    register_a = register_b>>register_a;
}

void adj(int val) {
    stack_pointer+=val;
}


void a2sp(int val) {
    stack_pointer = register_a;
    register_a = register_b;
}

void sp2a(int val) {
    register_b = register_a;
    register_a = stack_pointer;
}

void call(int val) {
    register_b = register_a;
    register_a = program_counter;
    program_counter+=(val);
}

void ret(int val) {
    program_counter = register_a;
    register_a = register_b;
}
void brz(int val) {
    if (register_a == 0) program_counter += (val);
}

void brlz(int val) {
    if (register_a < 0) program_counter += (val);
}

void br(int val) {
    program_counter+=val;
}

void halt(int val ) {
    return;
}

const char *instruction_list[19] = {"ldc", "adc", "ldl", "stl", "ldnl", "stnl", "add", "sub", "shl", "shr", "adj", "a2sp", "sp2a", "call", "return", "brz", "brlz", "br", "HALT"};
void (*function_pointers[19])(int val) = {ldc, adc, ldl, stl, ldnl, stnl, add, sub, shl, shr, adj, a2sp, sp2a, call, ret, brz, brlz, br, halt};
/* create an array of function pointers (if possible it would be much easier than if and else); */
int fetch (FILE** fin) {
    int instruction = 0;
    fread(&instruction, sizeof(int), 1, *fin);
    return instruction;
}
void decode_and_execute(int instruction) {
    int opcode = (instruction&0xFF);
    int val = instruction>>8;
    if (opcode > 18) return;
    printf("%08X  %s\n",instruction, instruction_list[opcode]);
    if (!strcmp(instruction_list[opcode], "HALT")) return;
    (*function_pointers[opcode])(val);
}
int loader(FILE** fin) {
    int size = 0;
    while(!feof(*fin)) {
        int instruction = 0;
        if (size > memory_size) {
            printf("OUT OF MEMORY");
            break;
        };
        instruction  = fetch(fin);
        memory_store[size++] = instruction;
    }
    return size-1;
}

void print_memory(int size) {
    int i = 0;
    for (i;i<size;i++) printf("%08X\n", memory_store[i]);
}

void run_program(int size) {
    for (;program_counter<size;program_counter++) {
        decode_and_execute(memory_store[program_counter]);
    }
}
int main(int argc, char* argv[]) {
    FILE* fptr;
    int programSize;
    if (argc <= 2) {
        printf("usage: emu [options] file.o\n-trace   show instruction trace\n-read   show memory reads\n-write     show memory writes\n-before     show memory dump before execution\n-after      show memory dump after execution\n-wipe wipe written flags before execution\n-isa      display isa\n");
    } else {
        printf("%s\n", argv[2]);
        fptr = fopen(argv[2], "rb");
        if (fptr == 0) printf("Unable to read the executable");
       else {
            programSize = loader(&fptr);
           if (!strcmp(argv[1], "-trace")) {
               printf("Program loaded to memory, You can execute it line by line by pressing any character, q will quit the program\n");
               char ch;
               while(ch) {
                   
               }
           } else if (!strcmp(argv[1], "-read")) {

           } else if (!strcmp(argv[1], "-write")) {

           } else if (!strcmp(argv[1], "-before")) {
               /* Simply print the whole memory content */
                print_memory(programSize);
           } else if (!strcmp(argv[1], "-after")) {
               /* First execute the program then show the memory */
                printf("Executing the program first, if there is an infinite it will get stuck\n");
                run_program(programSize);
                /* Exexuted printing memory contents */
                printf("Executed!!! now printing memory contents\n");
                print_memory();
           } else if (!strcmp(argv[1], "-wipe")) {

           } else if (!strcmp(argv[1], "-isa")) {
           } 
           /* Load the program to the local memory  and get size of program*/
            for (;program_counter<programSize;program_counter++) {
                decode_and_execute(memory_store[program_counter]);
            }
        }
    }
    return 0;
}