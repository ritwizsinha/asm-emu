/* RITWIZ SINHA
1801CS39
I hereby declare that all this code is written by me and me alone */
struct warning{
    char* msg;
    int line;
} warnings[MAX_WARNING_SIZE];

void push_warnings(char* errorMsg, int pc) {
    warnings[warning_index].line = pc;
    warnings[warning_index].msg = errorMsg;
    warning_index++;
}