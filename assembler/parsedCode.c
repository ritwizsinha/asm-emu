#define MAX_CODE_SIZE 1000

struct parsedCodeLine {
    int addr;
    char *label;
    struct OP op;
    char *operand; 
    char* comment;
} parsedCode[MAX_CODE_SIZE];

void initParsedCode() {
    int i=0;
    for(;i<MAX_CODE_SIZE;i++) {
        parsedCode[i].addr = -1;
        parsedCode[i].comment = '\0';
        parsedCode[i].label = '\0';
        parsedCode[i].op.str = '\0';
        parsedCode[i].op.size = 0;
        parsedCode[i].op.opcode = -1;
        parsedCode[i].operand = '\0';
    }
}