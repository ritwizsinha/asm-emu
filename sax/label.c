/* This label structure stores the labels encountered in a particular program 
str is the character pointer to the Label
addr is the address at which the label is found
used specifies whether the label was used somewhere as an argument or not( Useful for giving a warning)
*/
struct Label {
    char *str;
    int addr;
    int used;
    int codelineIndex;
} labels[lArrSize];

/* Checks if a label exists in the labels table */
int checkLabelExists(const char* label) {
    int i = 0;
    for (;i<label_array_index;i++) 
        if (!strcmp(label, labels[i].str)) return 1;
    return 0;
}

/* Checking if the label is bogus ie the label starts with a digit or its size is 0 */
int bogusLabel(char* label) {
    if (label == 0) return 1;
    if (isdigit(label[0])) return 1;
    return 0;
}

/* Check if there exists a label in the code line by checking for ':' */
int hasLabel(const char* line) {
    char* ptr = strchr(line, ':');
    return (ptr == 0 ? -1 : ptr - line); 
}

/* Store the label in the labels array */
void pushLabel(char* line, int delimeter, int index) {
    int k = 0;
    labels[label_array_index].str = (char*)malloc(sizeof(char)*MAX_LABEL_SIZE);
    for(;k < delimeter;k++) labels[label_array_index].str[k] = line[k];
    labels[label_array_index].addr = -1;
    labels[label_array_index].used = 0;
    labels[label_array_index].codelineIndex = index;
    label_array_index++;
}

int findLabelAddress(char* label) {
    int i=0;
    for(;i<lArrSize;i++) {
        if (!strcmp(label, labels[i].str)) return labels[i].addr;
    }
    return -1;
}

int findLabelCodeLineIndex(char* label) {
    int i=0;
    for(;i<lArrSize;i++) {
        if (!strcmp(label, labels[i].str)) return labels[i].codelineIndex;
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