/* RITWIZ SINHA
1801CS39
I hereby declare that all this code is written by me and me alone */
/* Checks if a comment exists in a given line by checking for ';' */
int hasComment(const char* line) {
    char* ptr = strchr(line, ';');
    return (ptr == 0 ? -1 : ptr - line); 
}
/* Removes the comment from the given line and returns the modified line */
char* removeComment(const char* line, int delimeter) {
    char* tmp = (char*)(malloc(sizeof(char)*(delimeter+1)));
    int i = 0;
    for(;i<delimeter;i++) tmp[i] = line[i];
    return tmp;
}
