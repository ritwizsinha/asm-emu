#define MAX_ERROR_SIZE 100
struct Error {
    char* msg;
    int line;
} errors[MAX_ERROR_SIZE];
int error_list_index=0;
/* List of Lines already having errors , so that new errors are ignored 
For eg if instr is ldc 3,4 
and ldc doesn't take any arguments then it is 
not useful to tell whether 3,4 is a valid format or not
*/
int ignoreList[MAX_ERROR_SIZE];
int ignore_list_iterator = 0;
void push_errors(char* errorMsg, int pc) {
    int i = 0;
    for (;i<ignore_list_iterator;i++) if (pc == ignoreList[i]) return;
    ignoreList[ignore_list_iterator++] = pc;
    errors[error_list_index].line = pc;
    errors[error_list_index].msg = errorMsg;
    error_list_index++;
}

void show_errors() {
    int i = 0;
    while(i < error_list_index) {
        printf("Error, Line %d : %s\n", errors[i].line+1, errors[i].msg);
        i++;
    }
}

