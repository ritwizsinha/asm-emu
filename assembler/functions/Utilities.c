int getSize(char* line) {
    int i=0;
    while(line[i]!='\0') i++;
    return i;
}

char* removeWhiteSpace(char* line, int size) {
    int l = 0;
    int i = 0;
    for(;i<size;i++) if (line[i] != ' ' && line[i] != '\t') line[l++] = line[i];
    line[l] = '\0';
    return line;
}

int getOperation(char* line, int delimeter, int size) {
    int i = 0;
    if (delimeter >= size ) return -1;
   delimeter = delimeter == -1 ?  0 : delimeter+1;
    for(;i<MNEMONIC_STR_SIZE;i++) {
        char* tmp = mnemonics[i].str;
        int sz = mnemonics[i].size;
        if (sz > size-delimeter) continue;
        int index = 0;
        while (line[index + delimeter]!='\0' && tmp[index]!='\0'&&
        line[index+delimeter] == tmp[index]) index++;
        /*printf("Index: %d, Operation: %s\n", index, tmp);*/
        if (index== sz) return i;
    }
    return -1;
}
