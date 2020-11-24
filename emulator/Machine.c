#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#define memory_size 10000

int exit_loop = 0;
int memory_store[memory_size];
int size = 0;

struct write_mem {
    int addr;
    int prev;
    int cur;
} write_memory_array[memory_size];
int write_memory_array_index = 0;

struct read_mem {
    int addr;
    int cur;
} read_memory_array[memory_size];
int read_memory_array_index = 0;
/* Internal State */
int register_a = 0 ,register_b = 0, stack_pointer = 0, program_counter = 0;

void write_memory(int address, int previous, int cur) {
        write_memory_array[write_memory_array_index].addr = address;
        write_memory_array[write_memory_array_index].prev = previous;
        write_memory_array[write_memory_array_index].cur = cur;
        write_memory_array_index++;
}
void read_memory(int address, int cur) {
        read_memory_array[read_memory_array_index].addr = address;
        read_memory_array[read_memory_array_index].cur = cur;
        read_memory_array_index++;
}
void ldc(int val) {
    register_b = register_a;
    register_a = val;
    program_counter++;
}

void adc(int val) {
    register_a+=val;
    program_counter++;
}

void ldl(int val) {
    register_b = register_a;
    if (stack_pointer + val < memory_size) {
        read_memory(stack_pointer + val, memory_store[stack_pointer + val]);
        register_a = memory_store[stack_pointer + val];
    } 
    program_counter++;
}

void stl (int val) {
    if (stack_pointer + val < memory_size) {
        write_memory(stack_pointer + val, memory_store[stack_pointer + val], register_a);
        memory_store[stack_pointer + val] = register_a;
        register_a = register_b;
    }
    program_counter++;
} 

void ldnl(int val) {
    if (register_a + val < memory_size) {
        read_memory(register_a + val, memory_store[register_a + val]);
        register_a = memory_store[register_a + val];
    }
    program_counter++;
}

void stnl(int val) {
    if (register_a + val< memory_size) {
        write_memory(register_a + val, memory_store[register_a + val], register_b);
        memory_store[register_a + val]  = register_b;
    }
    program_counter++;
}

void add(int val) {
    register_a +=register_b;
    program_counter++;
}

void sub(int val) {
    register_a = register_b - register_a;
    program_counter++;
}

void shl(int val) {
    register_a = register_b<<register_a;
    program_counter++;
}

void shr(int val) {
    register_a = register_b>>register_a;
    program_counter++;
}

void adj(int val) {
    stack_pointer+=val;
    program_counter++;
}


void a2sp(int val) {
    stack_pointer = register_a;
    register_a = register_b;
    program_counter++;
}

void sp2a(int val) {
    register_b = register_a;
    register_a = stack_pointer;
    program_counter++;
}

void call(int val) {
    register_b = register_a;
    register_a = program_counter+1;
    program_counter+=(val+1);
}

void ret(int val) {
    program_counter = register_a;
    register_a = register_b;
}
void brz(int val) {
    if (register_a == 0) program_counter += (val+1);
    else program_counter++;
}

void brlz(int val) {
    if (register_a < 0) program_counter += (val+1);
    else program_counter++;
}

void br(int val) {
    program_counter+=(val+1);
}

void halt(int val ) {
    return;
}

const char *instruction_list[19] = {"ldc", "adc", "ldl", "stl", "ldnl", "stnl", "add", "sub", "shl", "shr", "adj", "a2sp", "sp2a", "call", "return", "brz", "brlz", "br", "HALT"};
const char* no_operand_list[7] = {"add", "sub", "shl", "shr", "a2sp", "sp2a", "return"};
const char* operand_present_list[11] = {"ldc", "adc", "ldl", "stl", "ldnl", "stnl", "adj", "call", "brz", "brlz", "br"};
const char* write_ops[2] = {"stl", "stnl"};
const char* read_ops[] = {"ldl", "ldnl"};
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
    for (;i<size;i++) printf("%08X\n", memory_store[i]);
}

void run_program(int size) {
    while(program_counter < size && program_counter >= 0) {
        decode_and_execute(memory_store[program_counter]);
        int opcode = (memory_store[program_counter]&0xFF);
        if (!strcmp(instruction_list[opcode], "HALT")) break;
    }
}
void clear_read_memory() {
    read_memory_array_index = 0;
}
void clear_write_memory() {
    write_memory_array_index = 0;
}
int print_instr(int instruction) {
        int opcode = (instruction&0xFF);
        int val = instruction>>8;
        int start = 0;
        if (opcode > 18) printf("%08X\n", instruction);
        else {
            int flag = 1;
            printf("%s ", instruction_list[opcode], val);
            for (;start<11;start++) if (!strcmp(operand_present_list[start], instruction_list[opcode])) {
                printf("%08X\n", val);
                flag = 0;
            }
            if (flag) printf("\n");
            if (!strcmp(instruction_list[opcode], "HALT")) {
                printf("HALT\n");
                return -1;
            }
        }
        return 0;
}
void print_program(int size) {
    for(;program_counter<size;program_counter++) {
        int res = print_instr(memory_store[program_counter]);
        if (res == -1) {
            printf("HALT\n");
            return; 
        }

    }
}
int main(int argc, char* argv[]) {
    FILE* fptr;
    int programSize;
    if (argc < 2) {
        printf("usage: emu [options] file.o\n-trace   show instruction trace\n-read   show memory reads\n-write     show memory writes\n-before     show memory dump before execution\n-after      show memory dump after execution\n-wipe wipe written flags before execution\n-isa      display isa\n");
    } else {
        fptr = fopen(argv[1], "rb");
        if (fptr == 0) printf("Unable to read the executable");
       else {
           /* Loads the program to memory */
            programSize = loader(&fptr);
            printf("Program Loaded\n");
            while(1) {
                char str[100];
                printf("Enter operation - ");
                scanf("%s", str);
            if (!strcmp(str, "trace")) {
                char ch='c';
                /* printf("Program loaded to memory, You can execute it line by line by pressing any character, q will quit the program, c will continue to end\n"); */
                int cnt = 0;
                int instructions = 0;
                while(ch!='q' && program_counter < programSize && program_counter >= 0) {
                    int res = 0;
                    int prev_program_counter_position;
                    instructions++;
                    prev_program_counter_position = program_counter;
                    decode_and_execute(memory_store[program_counter]);
                    printf("PC = %08X, SP = %08X, A = %08X, B = %08X ", program_counter, stack_pointer, register_a, register_b);
                    res = print_instr(memory_store[prev_program_counter_position]);
                    if (res == -1) break;
                    if (!cnt) {
                            scanf("\n%c",&ch); 
                            if (ch == 'c') cnt = 1;
                    }
                }
               printf("Program finished with %d instructions\n", instructions);
           } else if (!strcmp(str, "read")) {
                int i = 0;
                clear_read_memory();
                run_program(programSize);
                for (;i<read_memory_array_index;i++) {
                    printf("Reading memory [%08X] finds %08X\n", read_memory_array[i].addr, read_memory_array[i].cur);
                } 
           } else if (!strcmp(str, "write")) {
                int i = 0;
                clear_write_memory();
                run_program(programSize);
                for (;i<write_memory_array_index;i++) {
                    printf("Writing memory [%08X] was %08X now %08x\n", write_memory_array[i].addr, write_memory_array[i].prev, write_memory_array[i].cur);
                }
           } else if (!strcmp(str, "before")) {
               /* Simply print the whole memory content */
                print_memory(programSize);
           } else if (!strcmp(str, "after")) {
               /* First execute the program then show the memory */
                printf("Executing the program first, if there is an infinite it will get stuck\n");
                run_program(programSize);
                /* Exexuted printing memory contents */
                printf("Executed!!! now printing memory contents\n");
                print_memory(programSize);
           } else if (!strcmp(str, "wipe")) {
               program_counter = 0;
               register_a = 0;
               register_b = 0;
               stack_pointer = 0;
               printf("Set program counter, stack_pointer, register_a and register_b to 0\n");
           } else if (!strcmp(str, "isa")) {
               print_program(programSize);
           }
           else
           {
               printf("Exiting\n");
               return 0;
           }
            }
    }
    }
    return 0;
} 