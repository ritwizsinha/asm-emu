/* Structure storing a separated and condensed structure of line 
addr stores the address in the meemory
label stores the label if any in that line
op Stores all characteristics of operator if present
opr Stores all characteristics of operand if present
comment Stores the comment if present
instrCode stores the machine code as a 32 bit integer
*/
struct parsedCodeLine {
    int addr;
    char *label;
    struct operator op;
    struct operand opr; 
    char* comment;
    int instrCode;
} parsedCode[MAX_CODE_SIZE];
/* Initialize the parsed code table */
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
        parsedCode[i].instrCode = 0;
    }
}
/* Function which checks the validity of operands , stores the errors if any and stores the validated operands */
void check_and_set_operand(int index, char* line, int size) {
    char* locationPtr = line;
    int num = -1;
    if (size == 0) return;
    if (index >= pc) return;
    /* Strtol finds the number in the string if present in hexadecimal, octal or decimal */
    num  = strtol(line, &locationPtr, 0);
    /* Check if it is a number */
    if (*locationPtr == '\0') {
        parsedCode[index].opr.isDigit = 1;
        parsedCode[index].opr.digit = num;
        parsedCode[index].opr.noOp = 0;
    } else if (checkLabelExists(line)) {
        /* Check if operand is a label */
        parsedCode[index].opr.isLabel = 1;
        parsedCode[index].opr.noOp = 0;
    } else {
        /* If none then show an error */
        push_errors("Operand is not defined", parsedCode[index].addr);
    }
}

/* Check for errors of needed and not needed operands for a particular operator */
void check_operators(int index) {
    struct operand  oprd = parsedCode[index].opr;
    struct operator op = parsedCode[index].op;
    if (op.op_req && oprd.noOp) push_errors("Operand required", parsedCode[index].addr);
    if (!op.op_req && !oprd.noOp) push_errors("Operand not required", parsedCode[index].addr);
}
/* Validating and storing a line to the parsed code table */
void check_and_set_line(int index) {
    char* operand = parsedCode[index].opr.op;
    /* Priority wise first check errors in the operands */
    check_and_set_operand(index, operand, getSize(operand));
    /* If operands are correct and find errors in teh operators */
    check_operators(index);
} 

/* Find the machine code of digit or offset of label for machine code */
void storeLabelOrData(int* instr, struct parsedCodeLine tmp) {
    if (tmp.opr.isDigit) {
        if (numInRange(tmp.opr.digit)) {
            /* Store the digit to the most significant 24 bits */
            (*instr) |= (tmp.opr.digit<<8);
        } else {
            /* Store errors if number out of range */
            push_errors("Number out of range", tmp.addr);
        }
    } else if (tmp.opr.isLabel) {
        int labelAddress = findLabelAddress(tmp.opr.op);
        /* Find the offset of label from the next address */
        int offset = labelAddress-tmp.addr-4;
        if (numInRange(offset)) {
            /* Store the offset in the most significant 24 bits */
            (*instr) |= (offset<<8);
        } else {
            push_errors("Label not in range", tmp.addr);
        }
    }
}

/* Assign valid address to instruction lines */
void assign_valid_address() {
    int address = 0;
    int i = 0;
    for(;i<pc;i++) {

        if (parsedCode[i].label || parsedCode[i].op.str || parsedCode[i].opr.op) {
            if (parsedCode[i].label) {
                int index = findLabelIndex(parsedCode[i].label);
                labels[index].addr = address;
            }
            parsedCode[i].addr = address;
            if (parsedCode[i].op.str) {
                /* Increment the address pointer only if the current line has an operand */
                address+=4;
            }
        } else parsedCode[i].addr = -1;
    }
}
/* Assign machine code for the directives and the opcodes */
void assignInstr() {
    int i = 0;
    for(;i<pc;i++) {
        int instruction = 0;
        int labelAddress = 0;
        int findOffset = 0;
        if (parsedCode[i].addr == -1) continue;
        if (parsedCode[i].op.str) {
            if (!strcmp(parsedCode[i].op.str,"data") || !strcmp(parsedCode[i].op.str, "SET")) {
                if (parsedCode[i].opr.isLabel) {
                    labelAddress = findLabelAddress(parsedCode[i].opr.op);
                    findOffset = labelAddress-parsedCode[i].addr-4;
                    if (numInRange32(findOffset)) {
                        instruction = findOffset;
                    } else {
                        push_errors("The label offset is out of range", parsedCode[i].addr);
                    }
                } else if(parsedCode[i].opr.isDigit) {
                    if (numInRange32(parsedCode[i].opr.digit)) {
                        instruction |= ((parsedCode[i].opr.digit)<<8);
                    } else {
                        push_errors("Number out of range", parsedCode[i].addr);
                    }
                }
            }
            else {
                instruction += parsedCode[i].op.opcode;
                if (parsedCode[i].op.op_req) {
                    storeLabelOrData(&instruction, parsedCode[i]);
                }
            }
        }
        parsedCode[i].instrCode = instruction;
    }
}

void storeLabel(int pc, char* line, int delimeter) {
    int k = 0;
    parsedCode[pc].label = (char*)(malloc(sizeof(char)*MAX_LABEL_SIZE));
    for(;k < delimeter;k++)  parsedCode[pc].label[k] = line[k];
}

void storeOperation(int opcodeIndex, int pc) {
    if (opcodeIndex == -1) return;
    parsedCode[pc].op = mnemonics[opcodeIndex];
}

void storeOperand(int pc, int index, char* line) {
    int size = index;
    for(;line[size]!='\0';size++);
    parsedCode[pc].opr.op = (char*)(malloc(sizeof(char)*(size+1)));
    strcpy(parsedCode[pc].opr.op, line+index); 
}