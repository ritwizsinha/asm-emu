void createListingFile(char* fileName) {
    char listingFileName[50];
    FILE* fwrite;
    int index = 0;
    strcpy(listingFileName, fileName);
    strcat(listingFileName, ".l");
    fwrite = fopen(listingFileName, "w");
    for(;index<pc;index++) {
        struct parsedCodeLine tmp = parsedCode[index]; 
        if (tmp.addr == -1) {
            fprintf(fwrite, "; %s\n", tmp.comment);
        } else {
            fprintf(fwrite, "%08x\t %08x\t", tmp.addr, tmp.instrCode);
            if (tmp.label) {
                fprintf(fwrite, "%s:\t", tmp.label);
            } 
            if (tmp.op.str)  {
                fprintf(fwrite, "%s ", tmp.op.str);
            }
            if (tmp.op.op_req && !tmp.opr.isDigit) {
                fprintf(fwrite, "%s:\t", tmp.opr.op);
            } else if (tmp.op.op_req) {
                fprintf(fwrite, "%d\t", tmp.opr.digit);
            }
            if (tmp.comment) {
                fprintf(fwrite, ";%s", tmp.comment);
            }
            fprintf(fwrite, "\n");
        }
    }
    fclose(fwrite);
}

void createMachineCodeFile(char* fileName) {
    char machineCodeFileName[50];
    FILE* fout;
    int i = 0;
    strcpy(machineCodeFileName, fileName);
    strcat(machineCodeFileName, ".o");
    printf("%s\n", machineCodeFileName);
    fout = fopen(machineCodeFileName, "wb");
    for(;i<pc;i++) {
        if (parsedCode[i].addr == -1) continue;
        fwrite(&parsedCode[i].instrCode, sizeof(int), 1, fout);
    }
    fclose(fout);
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
