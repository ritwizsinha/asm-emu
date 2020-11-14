int getSize(char* line) {
    int i=0;
    while(line[i]!='\0') i++;
    return i;
}

void validate_line(char* line) {
    int i = 0;
    char* possibleLanguage = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789 \t:;";
    /* TO check no unwanted characters appear */
    int colonCount = 0;
    while(line[i] != '\0') {
        int flag = 0,j=0;
        while(possibleLanguage[j]!='\0') {
            if (line[i] == possibleLanguage[j]) flag=1;
            j++;
        }
        i++;
        if (line[i] == ':') colonCount++;
        if (!flag) push_errors("Unrecognized character found", pc);
    }
    if (colonCount > 1) push_errors("Found more than one colon on a line", pc);
}
int compareString(char* str1, int size1, char* str2, int size2) {
    int i=0;
    if (size1 != size2) return 0;
    for(;i<size1;i++) if (str1[i] != str2[i]) return 0;
    return 1;
}

char* removeWhiteSpace(char* line, int size) {
    int l = 0;
    int i = 0;
    for(;i<size;i++) if (line[i] != ' ' && line[i] != '\t') line[l++] = line[i];
    line[l] = '\0';
    return line;
}

