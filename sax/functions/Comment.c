int hasComment(char* line, int size) {
    int i = 0;
    for(;i<size;i++) if (line[i] == ';') return i;
    return -1;
}

char* removeComment(char* line, int delimeter) {
    line[delimeter] = '\0';
    return line;
}
