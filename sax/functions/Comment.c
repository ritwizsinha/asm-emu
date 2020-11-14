int hasComment(char* line) {
    int i = 0;
    for(;line[i]!='\0';i++) if (line[i] == ';') return i;
    return -1;
}

char* removeComment(char* line, int delimeter) {
    line[delimeter] = '\0';
    return line;
}
