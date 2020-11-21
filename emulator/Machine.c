#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

int exit_loop = 0;
int* machine_code_store = 0;
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
    register_a = stack_pointer + val >= size ? 0 : machine_code_store[stack_pointer + val];
}

void stl (int val) {
    if (stack_pointer + val < size) {
        machine_code_store[stack_pointer + val] = register_a;
        register_a = register_b;
    }
} 

void ldnl(int val) {
    register_a = register_a + val >= size ? 0 : machine_code_store[register_a + val];
}

void stnl(int val) {
    if (register_a + val< size) machine_code_store[register_a + val]  = register_b;
}

void add(int val) {
    register_a +=register_b;
}

void sub(int val) {
    register_a-=register_b;
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
    int val = instruction - opcode;
    if (opcode > 18) return;
    printf("%08X  %s\n",instruction, instruction_list[opcode]);
    if (!strcmp(instruction_list[opcode], "HALT")) return;
    (*function_pointers[opcode])(val);
}
int main(int argc, char* argv[]) {
    FILE* fptr;

    if (argc < 2) printf("Please enter the executable");
    fptr = fopen(argv[1], "rb");
    if (fptr == 0) printf("Unable to read the executable");
    /* First read to store all machine code in array */
    for (;size++;!feof(fptr)) {
        int instruction = 0;
        machine_code_store[size] = (int*)(malloc(sizeof(int)));
        instruction  = fetch(&fptr);
        machine_code_store[size] = instruction;
    }
    for (;program_counter<size;program_counter++) {
        decode_and_execute(machine_code_store[program_counter]);
    }
    return 0;
}