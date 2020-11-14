#define lArrSize 100

struct Label {
    char *str;
    int addr;
    int used;
} labels[lArrSize];

int labelArrayIndex = 0;
int checkLabelExists(char* label, int size) {
    int i = 0;
    for (;i<labelArrayIndex;i++) {
        int sizeOfLabel = getSize(labels[i].str);
        if (compareString(label, size, labels[i].str, sizeOfLabel)) return 1;
    }
    return 0;
}
int bogusLabel(char* label, int size) {
    if (size == 0) return 1;
    if (isdigit(label[0])) return 1;
    return 0;
}
char* getLabel(char* line, int delimiter) {
    int has_error = 0;
    char* label = (char*)malloc(20*sizeof(char));
    int k = 0;
    label[0] = '\0';
    while (k < delimiter) {
        if (!isalnum(line[k]) && !has_error) push_errors("Label not alphanumeric" , pc), has_error=1;
        label[k] = line[k];
        k++;
    }
    if (checkLabelExists(label, k)) push_errors("Duplicate label found", pc);
    return label;
}

int hasLabel(char* line) {
    int i = 0;
    for(;line[i]!='\0';i++) if (line[i] == ':') return i;
    return -1;
}

void storeLabel(char *label, int programCounterAddr) {
    labels[labelArrayIndex].str = label;
    labels[labelArrayIndex].addr = programCounterAddr;
    labels[labelArrayIndex].used = 0;
}

/*
int isValidLabel(char* label, int size) {
    
}
*/