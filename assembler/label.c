#define lArrSize 100

struct Label {
    char *str;
    int addr;
} labels[lArrSize];

int labelArrayIndex = 0;

char* getLabel(char* line, int delimiter) {
    char* label = (char*)malloc(20*sizeof(char));
    label[0] = '\0';
    int k = 0;
    while (k < delimiter) {
        label[k] = line[k];
        k++;
    }
    return label;
}
int hasLabel(char* line, int size) {
    int i = 0;
    for(;i<size;i++) if (line[i] == ':') return i;
    return -1;
}

void storeLabel(char *label, int programCounterAddr) {
    labels[labelArrayIndex].str = label;
    labels[labelArrayIndex].addr = programCounterAddr;
}
