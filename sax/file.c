void createListingFile(char* fileName) {
    char listingFileName[50];
    FILE* fwrite;
    int index = 0;
    strcpy(listingFileName, fileName);
    strcat(listingFileName, ".l");
    fwrite = fopen(listingFileName, "w");
    for(;index<pc;index++) {
        struct parsedCodeLine tmp = parsedCode[index]; 
/*         printf("Address: %d\nComment: %s\nLabel: %s\nOperand Required: %d\nOpcode: %d\nOperator Size: %d\nOperator: %s\nOperand: %s\nIsLabel: %d\nIsDigit: %d\nDigit: %d\nNo operation: %d\n\n",
        tmp.addr,
        tmp.comment,
        tmp.label,
        tmp.op.op_req,
        tmp.op.opcode,
        tmp.op.size,
        tmp.op.str,
        tmp.opr.op,
        tmp.opr.isLabel,
        tmp.opr.isDigit,
        tmp.opr.digit,
        tmp.opr.noOp
        ); */
        
    }
    fclose(fwrite);
}

void createMachineCodeFile(char* fileName) {
    char machineCodeFileName[50];
    strcpy(machineCodeFileName, fileName);
    strcat(machineCodeFileName, ".o");
    printf("%s\n", machineCodeFileName);
}
void createErrorFile(char* fileName) {
    char errorFileName[50];
    FILE* fwrite;
    int i = 0;
    strcpy(errorFileName, fileName);
    strcat(errorFileName, ".log");
    fwrite = fopen(errorFileName, "w");
    for(;i<error_list_index;i++) {
        fprintf(fwrite, "Error Line: %d, %s\n", errors[i].line, errors[i].msg);
    }
    fclose(fwrite);
}
void createFile(char* fileName) {
    if (error_list_index == 0) {
        createListingFile(fileName);
        createMachineCodeFile(fileName);
    } else {
        createErrorFile(fileName);
    }
}
