struct warning{
    char* msg;
    int line;
} warnings[MAX_WARNING_SIZE];

void push_warnings(char* errorMsg, int pc) {
    warnings[warning_index].line = pc;
    warnings[warning_index].msg = errorMsg;
    warning_index++;
}