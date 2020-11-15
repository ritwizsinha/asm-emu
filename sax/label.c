/* This label structure stores the labels encountered in a particular program 
str is the character pointer to the Label
addr is the address at which the label is found
used specifies whether the label was used somewhere as an argument or not( Useful for giving a warning)
*/
struct Label {
    char *str;
    int addr;
    int used;
} labels[lArrSize];

/* Checks if a label exists in the labels table */
int checkLabelExists(char* label) {
    int i = 0;
    for (;i<labelArrayIndex;i++) 
        if (!strcmp(label, labels[i].str)) return 1;
    return 0;
}

/* Checking if the label is bogus ie the label starts with a digit or its size is 0 */
int bogusLabel(char* label, int size) {
    if (size == 0) return 1;
    if (isdigit(label[0])) return 1;
    return 0;
}

/* Extracting the label from a code line */
char* getLabel(char* line, int delimiter) {
    int has_error = 0;
    char* label = (char*)malloc(20*sizeof(char));
    int k = 0;
    label[0] = '\0';
    while (k < delimiter) {
        /* Store error if label is not alphanumeric */
        if (!isalnum(line[k]) && !has_error) push_errors("Label not alphanumeric" , pc), has_error=1;
        label[k] = line[k];
        k++;
    }
    /* Store error if there is a duplicate label */
    if (checkLabelExists(label)) push_errors("Duplicate label found", pc);
    return label;
}

/* Check if there exists a label in the code line by checking for ':' */
int hasLabel(char* line) {
    int i = 0;
    for(;line[i]!='\0';i++) if (line[i] == ':') return i;
    return -1;
}

/* Store the label in the labels array */
void storeLabel(char *label, int programCounterAddr) {
    labels[labelArrayIndex].str = label;
    labels[labelArrayIndex].addr = programCounterAddr;
    labels[labelArrayIndex].used = 0;
}

int findLabelAddress(char* label) {
    int i=0;
    for(;i<lArrSize;i++) {
        if (!strcmp(label, labels[i].str)) return labels[i].addr;
    }
    return -1;
}

int findLabelIndex(char *label) {
        int i=0;
    for(;i<lArrSize;i++) {
        if (!strcmp(label, labels[i].str)) return i;
    }
    return -1;
}
/*
int isValidLabel(char* label, int size) {
    
}
*/