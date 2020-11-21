/* RITWIZ SINHA
1801CS39
I hereby declare that all this code is written by me and me alone */
/* Function for creating listing file */
void createListingFile(char* fileName) {
    /* Character array for storing the filename */
    char listingFileName[MAX_FILENAME_SIZE];
    FILE* fwrite;
    int index = 0;
    /* Getting the filename */
    strcpy(listingFileName, fileName);
    /* Adding extension for listing file */
    strcat(listingFileName, ".list");
    /* Opening for writing */
    fwrite = fopen(listingFileName, "w");
    for(;index<pc;index++) {
        struct parsedCodeLine tmp = parsedCode[index]; 
        if (tmp.addr == -1) {
            fprintf(fwrite, "; %s\n", tmp.comment);
        } else {
            fprintf(fwrite, "%04X\t %08X\t", tmp.addr, tmp.instrCode);
            if (tmp.label) {
                fprintf(fwrite, "%s:\t", tmp.label);
            } 
            if (tmp.op.str)  {
                fprintf(fwrite, "%s ", tmp.op.str);
            }
            if (tmp.op.op_req) {
                fprintf(fwrite, "%s\t", tmp.opr.op);
            }
            if (tmp.comment) {
                fprintf(fwrite, ";%s", tmp.comment);
            }
            fprintf(fwrite, "\n");
        }
    }
    fclose(fwrite);
}
/* Function for creating machine code file */
void createMachineCodeFile(char* fileName) {
    char machineCodeFileName[MAX_FILENAME_SIZE];
    FILE* fout;
    int i = 0;
    strcpy(machineCodeFileName, fileName);
    strcat(machineCodeFileName, ".o");
    fout = fopen(machineCodeFileName, "wb");
    for(;i<pc;i++) {
        if (parsedCode[i].addr == -1 || (parsedCode[i].label && !parsedCode[i].op.str)) continue;
        fwrite(&parsedCode[i].instrCode, sizeof(int), 1, fout);
    }
    fclose(fout);
}

/* Function for creating error file */
void createErrorFile(char* fileName) {
    char errorFileName[MAX_FILENAME_SIZE];
    FILE* fwrite;
    int i = 0;
    strcpy(errorFileName, fileName);
    strcat(errorFileName, ".log");
    fwrite = fopen(errorFileName, "w");
    for(;i<error_list_index;i++) {
        fprintf(fwrite, "Error Line: %d, %s\n", errors[i].line+1, errors[i].msg);
    }
    fclose(fwrite);
}
/* Function for creating warning file */
void createWarningFile(char* fileName) {
    char warningFileName[MAX_FILENAME_SIZE];
    FILE* fwrite;
    int i = 0;
    strcpy(warningFileName, fileName);
    strcat(warningFileName, ".warn");
    fwrite = fopen(warningFileName, "w");
    for(;i<warning_index;i++) {
        fprintf(fwrite, "Warning Line: %d, %s\n", warnings[i].line+1, warnings[i].msg);
    }
    fclose(fwrite);
}
/* Utility function for selecting whether error file should be created or not */
void createFile(char* fileName) {
    if (error_list_index == 0) {
        createWarningFile(fileName);
        createListingFile(fileName);
        createMachineCodeFile(fileName);
    } else {
        createErrorFile(fileName);
    }
}
