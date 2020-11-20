int hasComment(const char* line) {
    char* ptr = strchr(line, ';');
    return (ptr == 0 ? -1 : ptr - line); 
}

char* removeComment(const char* line, int delimeter) {
    char* tmp = (char*)(malloc(sizeof(char)*(delimeter+1)));
    int i = 0;
    for(;i<delimeter;i++) tmp[i] = line[i];
    return tmp;
}
