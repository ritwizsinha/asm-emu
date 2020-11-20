/* RITWIZ SINHA
1801CS39
I hereby declare that all this code is written by me and me alone */
/* Structure for an organized view of the operator
str stores the actual operator in string
size is the size of the string
opcode is the opcode of the mnemonic
op_req denotes whether this particular operator requires an operand or not */
struct operator {
    char* str;
    int size;
    int opcode;
    int op_req;
}  mnemonics[MNEMONIC_STR_SIZE];

/* Utility function for assigning mnemonic at a particular index */
void assignMnemonic(struct operator *ptr, char* str, int size, int opcode, int op_req) {
    (ptr)->opcode = opcode;
    (ptr)->size = size;
    (ptr)->str = str;
    (ptr)->op_req = op_req;
}

/* Prototype */
void getOperand(char* label, int size);

/* Store all the defined mnemonics */
void initMnemonicList() {
    assignMnemonic(mnemonics,"ldc", 3, 0, 1);
    assignMnemonic(mnemonics+1,"adc", 3, 1, 1);
    assignMnemonic(mnemonics+2,"ldl", 3, 2, 1);
    assignMnemonic(mnemonics+3,"stl", 3, 3, 1);
    assignMnemonic(mnemonics+4,"ldnl", 4, 4, 1);
    assignMnemonic(mnemonics+5,"stnl", 4, 5, 1);
    assignMnemonic(mnemonics+6,"add", 3, 6, 0);
    assignMnemonic(mnemonics+7,"sub", 3, 7, 0);
    assignMnemonic(mnemonics+8,"shl", 3, 8, 0);
    assignMnemonic(mnemonics+9,"shr", 3, 9, 0);
    assignMnemonic(mnemonics+10,"adj", 3, 10, 1);
    assignMnemonic(mnemonics+11,"a2sp", 4, 11, 0);
    assignMnemonic(mnemonics+12,"sp2a", 4, 12, 0);
    assignMnemonic(mnemonics+13,"call", 4, 13 , 1);
    assignMnemonic(mnemonics+14,"return", 6, 14, 0);
    assignMnemonic(mnemonics+15,"brz", 3, 15, 1);
    assignMnemonic(mnemonics+16,"brlz", 4, 16, 1);
    assignMnemonic(mnemonics+17,"br", 2, 17, 1);
    assignMnemonic(mnemonics+18,"HALT", 4, 18, 0);
    assignMnemonic(mnemonics+19, "data", 4, -1, 1);
    assignMnemonic(mnemonics+20, "SET", 3, -1, 1);
}

/* Get the index of a particular mnemonic in the mnemonic table */
int getOperation(const char* line, int delimeter, int size) {
    int i = 0;
    if (delimeter >= size ) return -1;
    delimeter = delimeter == -1 ?  0 : delimeter+1;
    for(;i<MNEMONIC_STR_SIZE;i++) {
        char* tmp = mnemonics[i].str;
        int sz = mnemonics[i].size;
        int index = 0;
        if (sz > size-delimeter) continue;
        while (line[index + delimeter]!='\0' && tmp[index]!='\0'&&
        line[index+delimeter] == tmp[index]) index++;
        /*printf("Index: %d, Operation: %s\n", index, tmp);*/
        if (index== sz) return i;
    }
    return -1;
}

int string_a_mnemonic(char* line) {
    int i = 0;
    for(;i<MNEMONIC_STR_SIZE;i++) if (!strcmp(line, mnemonics[i].str)) return 1;
    return 0;
}