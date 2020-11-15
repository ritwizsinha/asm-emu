#define MAX_CODE_SIZE 1000

struct parsedCodeLine {
    int addr;
    char *label;
    struct operator op;
    struct operand opr; 
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
        parsedCode[i].opr.op = '\0';
        parsedCode[i].opr.isDigit = 0;
        parsedCode[i].opr.isLabel = 0;
        parsedCode[i].opr.digit = '\0';
        parsedCode[i].opr.noOp = 1;
    }
}

void check_and_set_operand(int index, char* line, int size) {
    char* locationPtr = line;
    int num = -1;
    if (size == 0) return;
    if (index >= pc) return;
    num  = strtol(line, &locationPtr, 0);
    /* Check if it is a number */
    if (*locationPtr == '\0') {
        if (numInRange(num)) {
        parsedCode[index].opr.isDigit = 1;
        parsedCode[index].opr.digit = num;
        parsedCode[index].opr.noOp = 0;
        } else {
            push_errors("Number out of Range", parsedCode[index].addr);
        }
    } else if (checkLabelExists(line, size)) {
        parsedCode[index].opr.isLabel = 1;
        parsedCode[index].opr.noOp = 0;
    } else {
        push_errors("Operand is not defined", parsedCode[index].addr);
    }
}
void check_operators(int index) {
    struct operand  oprd = parsedCode[index].opr;
    struct operator op = parsedCode[index].op;
    if (op.op_req && oprd.noOp) push_errors("Operand required", parsedCode[index].addr);
    if (!op.op_req && !oprd.noOp) push_errors("Operand not required", parsedCode[index].addr);
}
void check_and_set_line(int index) {
    char* operand = parsedCode[index].opr.op;
    /* Priority wise first check errors in the operands */
    check_and_set_operand(index, operand, getSize(operand));
    /* If operands are correct and find errors in teh operators */
    check_operators(index);
} 