/* RITWIZ SINHA
1801CS39
I hereby declare that all this code is written by me and me alone */
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
void print(struct parsedCodeLine tmp) {
    printf("Address: %d\nComment: %s\nLabel: %s\nOperator: %s\nOperand: %s\nisDigit: %d\nisLabel: %d\n\n",
    tmp.addr, tmp.comment, tmp.label, tmp.op.str, tmp.opr.op, tmp.opr.isDigit, tmp.opr.isLabel);
}

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
void check_and_set_operand(int index, char* line) {
    char* locationPtr = line;
    long num = -1;
    int size = getSize(line);
    if (line == 0 || line[0] == '\0') return;
    if (index >= pc) return;
    /* Strtol finds the number in the string if present in hexadecimal, octal or decimal */
    num  = strtol(line, &locationPtr, 0);
    /* Check if it is a number */

    if (*locationPtr == '\0') {
        if (size > 2 && line[0] == '0' && line[1] == 'x') {
            if (!strcmp(parsedCode[index].op.str, "data") || !strcmp(parsedCode[index].op.str, "SET")){
                if (size > 10)
                    push_warnings("Overflow, data more than 32 bits", index);
            } 
            else if (size > 8) push_warnings("Overflow, offset/value more than 24 bits", index);
        } else if (size > 1 && line[0] == '0') {
            if (!strcmp(parsedCode[index].op.str, "data") || !strcmp(parsedCode[index].op.str, "SET")) {
                if (size > 11)
                    push_warnings("Overflow, data more than 32 bits", index);
            }
            else if (size > 9) push_warnings("Overflow, offset/value more than 24 bits", index);
        } else {
            if ((!strcmp(parsedCode[index].op.str, "data") || !strcmp(parsedCode[index].op.str, "SET"))) {
                if (!numInRange32(num))
                    push_warnings("Overflow, data more than 32 bits", index);
            }
            else if (!numInRange24(num)) push_warnings("Overflow, offset/value more than 24 bits", index);
        }
        parsedCode[index].opr.isDigit = 1;
        parsedCode[index].opr.digit = num;
        parsedCode[index].opr.noOp = 0;
    } else if (checkLabelExists(line)) {
        /* Mark the label as used */
        int labelPosition = findLabelIndex(line);
        labels[labelPosition].used = 1;
        /* Check if operand is a label */
        parsedCode[index].opr.isLabel = 1;
        parsedCode[index].opr.noOp = 0;
    } else {
        /* If none then show an error */
        push_errors("Operand is not defined", index);
    }
}

/* Check for errors of needed and not needed operands for a particular operator */
void check_operators(int index) {
    struct operand  oprd = parsedCode[index].opr;
    struct operator op = parsedCode[index].op;
    if (op.op_req && oprd.noOp) push_errors("Operand required", index);
    if (!op.op_req && !oprd.noOp) push_errors("Operand not required", index);
}
/* Validating and storing a line to the parsed code table */
void check_and_set_line(int index) {
    char* operand = parsedCode[index].opr.op;
    /* Priority wise first check errors in the operands */
    check_and_set_operand(index, operand);
    /* If operands are correct and find errors in teh operators */
    check_operators(index);
} 

/* Find the machine code of digit or offset of label for machine code */
void storeLabelOrData(int* instr, int index) {
    struct parsedCodeLine tmp = parsedCode[index];
    if (tmp.opr.isDigit) {
        if (numInRange24(tmp.opr.digit)) {
            /* Store the digit to the most significant 24 bits */
            (*instr) |= (tmp.opr.digit<<8);
        } else {
            /* Store errors if number out of range */
            push_warnings("Number out of range", index);
        }
    } else if (tmp.opr.isLabel) {
        int labelAddress = findLabelAddress(tmp.opr.op);
        /* Find the offset of label from the next address */
        int offset = labelAddress-tmp.addr-1;
        if (offset == -1) push_warnings("Infinite loop expected", index);
        if (offset == 0) push_warnings("Useless label", index);
        if (numInRange24(offset)) {
            /* Store the offset in the most significant 24 bits */
            (*instr) |= (offset<<8);
        } else {
            push_warnings("Label not in range", index);
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
                address+=1;
            }
        } else parsedCode[i].addr = -1;
    }
}
/* Assign machine code for the directives and the opcodes */
void assignInstr() {
    int i = 0;
    for(;i<pc;i++) {
        int instruction = 0;
        if (parsedCode[i].addr == -1) continue;
        if (parsedCode[i].op.str) {
            struct parsedCodeLine tmp = parsedCode[i];
            if (!strcmp(tmp.op.str, "SET")) {
                if (tmp.opr.isDigit) {
                    if (numInRange32(tmp.opr.digit)) 
                    instruction = tmp.opr.digit;
                    else push_warnings("Number out of range", i);
                } else {
                    push_errors("SET should only have a numeric value", i);
                }
            } 
            else if (!strcmp(tmp.op.str, "data")) {
                if (tmp.opr.isDigit) {
                    if (numInRange32(tmp.opr.digit)) 
                    instruction = tmp.opr.digit;
                    else push_warnings("Number out of range", i);
                } else {
                    push_errors("data should only have a numeric value", i);
                }
            } else if (!strcmp(tmp.op.str, "call") || !strcmp(tmp.op.str, "br") || !strcmp(tmp.op.str, "brz") || !strcmp(tmp.op.str, "brlz")) {
                instruction = tmp.op.opcode;
                storeLabelOrData(&instruction, i);
            } else {
                instruction = tmp.op.opcode;
                if (tmp.opr.isDigit) {
                    instruction |= (tmp.opr.digit<<8);
                } else if (tmp.opr.isLabel) {
                    int index = findLabelCodeLineIndex(tmp.opr.op);
                    if (!strcmp(parsedCode[index].op.str, "SET") || !strcmp(parsedCode[index].op.str, "data")) {
                        if (!strcmp(parsedCode[index].op.str, "SET"))
                            instruction |= (parsedCode[index].opr.digit<<8);
                        else instruction |= (findLabelAddress(parsedCode[i].opr.op)<<8);
                    } else{
                        push_warnings("Invalid label usage, Loading a code address", i);
                    }
                }
            }
            parsedCode[i].instrCode = instruction;
        }
    }
}

void storeLabel(int pc, char* line, int delimeter) {
    int k = 0;
    parsedCode[pc].label = (char*)(malloc(sizeof(char)*MAX_LABEL_SIZE));
    for(;k < delimeter;k++)  parsedCode[pc].label[k] = line[k];
    /* Checking for already existing label */
    if (checkLabelExists(parsedCode[pc].label)) {
        push_errors("Label already defined above", pc);
    }
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
/* Check for validity conditions on labels */
void check_label_valid() {
    int i = 0;
    for(;i<pc;i++) {
    /* Condition 1 */
        if (parsedCode[i].label && bogusLabel(parsedCode[i].label)) push_errors("Bogus Label found", i);
    /* Condition 2 */
        if (parsedCode[i].label && string_a_mnemonic(parsedCode[i].label)) push_errors("Label name cannot be a mnemonic name", i);
    }
}

void check_halt_exists_before_data_or_set() {
    int i = 0;
    int first_halt_occurence = -1;
    int halt_occurences = 0;
    int first_data_or_set_occurence = -1;
    int last_normal_mnemonic_occurence = -1;
    for(;i<pc;i++) { 
        if (!parsedCode[i].op.str) continue;
        if (!strcmp(parsedCode[i].op.str,"HALT")) {
            halt_occurences++;
            if (first_halt_occurence == -1)  first_halt_occurence = i;
        }
        else if (!strcmp(parsedCode[i].op.str,"data") || !strcmp(parsedCode[i].op.str,"SET")) {
            if (first_data_or_set_occurence == -1) first_data_or_set_occurence = i;
        } else last_normal_mnemonic_occurence = i;
    }
    if (first_data_or_set_occurence != -1 && first_halt_occurence > first_data_or_set_occurence) push_warnings("All data should be defined after halt, otherwise may lead to ambigous results", first_data_or_set_occurence);
    if (halt_occurences > 1) push_warnings("Multiple HALTS have no effect", -1);
    if (halt_occurences == 1 && last_normal_mnemonic_occurence > first_halt_occurence) push_warnings("Code after HALT is redundant", last_normal_mnemonic_occurence); 
}