#define MNEMONIC_STR_SIZE 21

struct OP {
    char* str;
    int size;
    int opcode;
}  mnemonics[MNEMONIC_STR_SIZE];

void assignMnemonic(struct OP *ptr, char* str, int size, int opcode) {
    (ptr)->opcode = opcode;
    (ptr)->size = size;
    (ptr)->str = str;
}

void initMnemonicList() {
    assignMnemonic(mnemonics,"ldc", 3, 0);
    assignMnemonic(mnemonics+1,"adc", 3, 1);
    assignMnemonic(mnemonics+2,"ldl", 3, 2);
    assignMnemonic(mnemonics+3,"stl", 3, 3);
    assignMnemonic(mnemonics+4,"ldnl", 4, 4);
    assignMnemonic(mnemonics+5,"stnl", 4, 5);
    assignMnemonic(mnemonics+6,"add", 3, 6);
    assignMnemonic(mnemonics+7,"sub", 3, 7);
    assignMnemonic(mnemonics+8,"shl", 3, 8);
    assignMnemonic(mnemonics+9,"shr", 3, 9);
    assignMnemonic(mnemonics+10,"adj", 3, 10);
    assignMnemonic(mnemonics+11,"a2sp", 4, 11);
    assignMnemonic(mnemonics+12,"sp2a", 4, 12);
    assignMnemonic(mnemonics+13,"call", 4, 13);
    assignMnemonic(mnemonics+14,"return", 6, 14);
    assignMnemonic(mnemonics+15,"brz", 3, 15);
    assignMnemonic(mnemonics+16,"brlz", 4, 16);
    assignMnemonic(mnemonics+17,"br", 2, 17);
    assignMnemonic(mnemonics+18,"HALT", 4, 18);
    assignMnemonic(mnemonics+19, "data", 4, -1);
    assignMnemonic(mnemonics+20, "SET", 3, -1);
}
