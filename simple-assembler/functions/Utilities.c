/* RITWIZ SINHA
1801CS39
I hereby declare that all this code is written by me and me alone */
/* Checks if a comment exists in a given line by checking for ';' */
/* Getting the size of the given string */
int getSize(const char* line) {
    int i=0;
    if (!line) return 0;
    while(line[i]!='\0') i++;
    return i;
}

/* Validates whether the given line assembly follows includes symbols only from the list of symbols that are allowed */
void validate_line(const char* line) {
    int i = 0;
    const char* possibleLanguage = ":;-_+ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789";
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

/* Removes all whitespace from a line including tabs and spaces*/
char* removeWhiteSpace(char* line) {
    int l = 0;
    int i = 0;
    for(;line[i]!='\0';i++) if (line[i] != ' ' && line[i] != '\t') line[l++] = line[i];
    line[l] = '\0';
    return line;
}

/* Checks whether a number is in range for a 24 bits number*/
int numInRange24(int num) {
    int MX = (1<<23)-1;
    int MN = -(1<<23);
    if (num > MX || num < MN) return 0;
    return 1;
}

/* Checks whether a number is in range for a 32 bits number*/
int numInRange32(int num) {
    int MX = ((long)1<<31)-1;
    int MN = -1 - MX;
    if (num > MX || num < MN) return 0;
    return 1;
}

/* Clears the string of all characters and places the null character */
void clearString(char* line, int size) {
    int i=0;
    for(;i<size;i++) line[i] = '\0';
}

/* Gets the filename from a path specified */
char* getFileName(char* line) {
    int size = getSize(line),dotIndex = -1, i=0, slashIndex=0, endIndex = -1, startIndex=-1;
    char* fileName = '\0';
    for(;i<size;i++) if (line[i] == '.') dotIndex = i;
    endIndex = dotIndex == -1 ? size-1 : dotIndex-1;
    for(i=0;i<size;i++) if (line[i] == '/') slashIndex = i;
    startIndex = slashIndex == 0 ? slashIndex : slashIndex+1;
    fileName = (char*)(malloc(sizeof(char)*(endIndex-startIndex+1)));
    for(i=startIndex;i<=endIndex;i++) fileName[i-startIndex] = line[i];
    return fileName;
}

